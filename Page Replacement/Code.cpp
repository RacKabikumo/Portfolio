# include <iostream>
# include <stdio.h>
# include <string>
# include <string.h>
# include <fstream> // 讀檔案
# include <vector> 
# include <queue>

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct Frame {
	int page_id ;
	string page_fault ;
	vector<int> page_frame ;
};

struct List {
	int count ;
	int num ;
};

class Mem {
	public :
	vector<int> origin ;
	vector<Frame> fifo ;
	vector<Frame> lru ;
	vector<Frame> lfu ;
	vector<Frame> mfu ;
	vector<Frame> lfu_lru ;
	vector<Frame> mfu_lru ;
	
	int frame_num ;
	vector<int> replace ;
	vector<int> fault ;
	
	void Init() {
		origin.clear() ;
		fifo.clear() ;
		lru.clear() ;
		lfu.clear() ;
		mfu.clear() ;
		lfu_lru.clear() ;
		mfu_lru.clear() ;
		frame_num = 0 ;
		replace.clear() ;
		fault.clear() ;
	} // Init()
	
	
	bool GetInput( string name ) {
		FILE *file = NULL ; // 檔案指標  
		string filename = name + ".txt" ;
		file = fopen( filename.c_str(), "r" ) ; 
		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
		if ( file == NULL ) {
			cout << endl << "檔案開啟失敗!" << endl << endl ; 
		} // if
		else {  
			int num = 0 ;
			int status = fscanf( file, "%d", &frame_num ) ;
			char c = '\0' ;
			status = fscanf( file, "%c", &c ) ;
			while ( c == '\n' ) {
				status = fscanf( file, "%c", &c ) ;
			} // if
			
			while ( status != -1 ) {
				if ( c >= '0' && c <= '9' ) {
					num = c - '0' ;
					origin.push_back( num ) ;
				} // if
				
				status = fscanf( file, "%c", &c ) ;
			} // while
			
		} // else 	
		
		fclose( file ) ;
	} // Input()
	
	void FIFO() {
		vector<int> list ;
		int i = 0 ;
		int change = 0 ;
		int fault_num = 0 ;
		while ( i < origin.size() ) {
			bool in_list = false ;
			Frame f ;
			f.page_id = origin.at( i ) ;
			for ( int j = 0 ; j < list.size() ; j++ ) { // 看有沒有在裡面 
				if ( origin.at( i ) == list.at( j ) ) {
					in_list = true ;
					f.page_fault = "" ;
					f.page_frame = list ;
				} // if
			} // for
			
			if ( !in_list ) {
				fault_num++ ;
				f.page_fault = "F" ;
				if ( list.size() < frame_num ) { // 有位置可以放 
					list.push_back( origin.at( i ) ) ; // 放進去啦	
					f.page_frame = list ;
				} // if
				else { // 要替換了 
					change++ ;
					list.erase( list.begin() ) ; // 拿掉最前面的
					list.push_back( origin.at( i ) ) ;
					f.page_frame = list ;
				} // else
			} // if
			
			fifo.push_back( f ) ; 
			
			i++ ;
		} // while
		
		fault.push_back( fault_num ) ;
		replace.push_back( change ) ;
	} // FIFO()
	
	void LRU() {
		vector<int> list ;
		int i = 0 ;
		int change = 0 ;
		int fault_num = 0 ;
		while ( i < origin.size() ) {
			bool in_list = false ;
			Frame f ;
			f.page_id = origin.at( i ) ;
			for ( int j = 0 ; j < list.size() ; j++ ) { // 看有沒有在裡面 
				if ( origin.at( i ) == list.at( j ) ) {
					list.erase( list.begin() + j ) ;
					list.push_back( origin.at( i ) ) ;
					in_list = true ;
					f.page_fault = "" ;
					f.page_frame = list ;
				} // if
			} // for
			
			if ( !in_list ) {
				fault_num++ ;
				f.page_fault = "F" ;
				if ( list.size() < frame_num ) { // 有位置可以放 
					list.push_back( origin.at( i ) ) ; // 放進去啦	
					f.page_frame = list ;
				} // if
				else { // 要替換了 
					change++ ;
					list.erase( list.begin() ) ; // 拿掉最前面的
					list.push_back( origin.at( i ) ) ;
					f.page_frame = list ;
				} // else
			} // if
			
			lru.push_back( f ) ; 
			
			i++ ;
		} // while
		
		fault.push_back( fault_num ) ;
		replace.push_back( change ) ;
	} // LRU()
	
	void LFU() {
		vector<List> list ;
		int i = 0 ;
		int change = 0 ;
		int fault_num = 0 ;
		while ( i < origin.size() ) {
			bool in_list = false ;
			List l ;
			Frame f ;
			f.page_id = origin.at( i ) ;
			for ( int j = 0 ; j < list.size() ; j++ ) { // 看有沒有在裡面 
				l = list.at( j ) ;
				if ( origin.at( i ) == l.num ) {
					list.at( j ).count++ ;
					in_list = true ;
					f.page_fault = "" ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
					
					break ;
				} // if
			} // for
			
			if ( !in_list ) {
				fault_num++ ;
				f.page_fault = "F" ;
				if ( list.size() < frame_num ) { // 有位置可以放 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					// 放進去啦	
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for

				} // if
				else { // 要替換了 
					change++ ;
					int min = 0 ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						if ( list.at( min ).count > list.at( j ).count ) {
							min = j ;
						} // if
					} // for
					
					list.erase( list.begin() + min ) ; // 拿掉count最小的 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
				} // else
			} // if
			
			lfu.push_back( f ) ; 
			
			i++ ;
		} // while
		
		fault.push_back( fault_num ) ;
		replace.push_back( change ) ;
	} // LFU()
	
	void MFU() {
		vector<List> list ;
		int i = 0 ;
		int change = 0 ;
		int fault_num = 0 ;
		while ( i < origin.size() ) {
			bool in_list = false ;
			List l ;
			Frame f ;
			f.page_id = origin.at( i ) ;
			for ( int j = 0 ; j < list.size() ; j++ ) { // 看有沒有在裡面 
				l = list.at( j ) ;
				if ( origin.at( i ) == l.num ) {
					list.at( j ).count++ ;
					in_list = true ;
					f.page_fault = "" ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
					
					break ;
				} // if
			} // for
			
			if ( !in_list ) {
				fault_num++ ;
				f.page_fault = "F" ;
				if ( list.size() < frame_num ) { // 有位置可以放 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					// 放進去啦	
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for

				} // if
				else { // 要替換了 
					change++ ;
					int max = 0 ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						if ( list.at( max ).count < list.at( j ).count ) {
							max = j ;
						} // if
					} // for
					
					list.erase( list.begin() + max ) ; // 拿掉count最小的 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
				} // else
			} // if
			
			mfu.push_back( f ) ; 
			
			i++ ;
		} // while
		
		fault.push_back( fault_num ) ;
		replace.push_back( change ) ;
	} // MFU()
	
	
	void LFU_LRU() {
		vector<List> list ;
		int i = 0 ;
		int change = 0 ;
		int fault_num = 0 ;
		while ( i < origin.size() ) {
			bool in_list = false ;
			List l ;
			Frame f ;
			f.page_id = origin.at( i ) ;
			for ( int j = 0 ; j < list.size() ; j++ ) { // 看有沒有在裡面 
				l = list.at( j ) ;
				if ( origin.at( i ) == l.num ) {
					l.count++ ;
					list.erase( list.begin() + j ) ;
					list.push_back( l ) ;
					in_list = true ;
					f.page_fault = "" ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
					
					break ;
				} // if
			} // for
			
			if ( !in_list ) {
				fault_num++ ;
				f.page_fault = "F" ;
				if ( list.size() < frame_num ) { // 有位置可以放 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					// 放進去啦	
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for

				} // if
				else { // 要替換了 
					change++ ;
					int min = 0 ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						if ( list.at( min ).count > list.at( j ).count ) {
							min = j ;
						} // if
					} // for
					
					list.erase( list.begin() + min ) ; // 拿掉count最小的 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
				} // else
			} // if
			
			lfu_lru.push_back( f ) ; 
			
			i++ ;
		} // while
		
		fault.push_back( fault_num ) ;
		replace.push_back( change ) ;
	} // LFU_LRU()
	
	
	void MFU_LRU() {
		vector<List> list ;
		int i = 0 ;
		int change = 0 ;
		int fault_num = 0 ;
		while ( i < origin.size() ) {
			bool in_list = false ;
			List l ;
			Frame f ;
			f.page_id = origin.at( i ) ;
			for ( int j = 0 ; j < list.size() ; j++ ) { // 看有沒有在裡面 
				l = list.at( j ) ;
				if ( origin.at( i ) == l.num ) {
					l.count++ ;
					list.erase( list.begin() + j ) ;
					list.push_back( l ) ;
					in_list = true ;
					f.page_fault = "" ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
					
					break ;
				} // if
			} // for
			
			if ( !in_list ) {
				fault_num++ ;
				f.page_fault = "F" ;
				if ( list.size() < frame_num ) { // 有位置可以放 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					// 放進去啦	
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for

				} // if
				else { // 要替換了 
					change++ ;
					int max = 0 ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						if ( list.at( max ).count < list.at( j ).count ) {
							max = j ;
						} // if
					} // for
					
					list.erase( list.begin() + max ) ; // 拿掉count最小的 
					l.num = origin.at( i ) ;
					l.count = 0 ;
					list.push_back( l ) ;
					f.page_frame.clear() ;
					for ( int j = 0 ; j < list.size() ; j++ ) {
						f.page_frame.push_back( list.at( j ).num ) ;
					} // for
				} // else
			} // if
			
			mfu_lru.push_back( f ) ; 
			
			i++ ;
		} // while
		
		fault.push_back( fault_num ) ;
		replace.push_back( change ) ;
	} // MFU_LRU()
	
	
	void Output( string name ) {
		FILE *file = NULL ; // 檔案指標 
		string filename = "out_" + name + ".txt" ;
		file = fopen( filename.c_str(), "w" ) ; 
		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
		if ( file == NULL ) {
			cout << endl << "檔案開啟失敗!" << endl << endl ; 
		} // if
		else {  
			fprintf( file, "--------------FIFO-----------------------\n" ) ;
			for ( int i = 0 ; i < fifo.size() ; i++ ) {
				Frame f = fifo.at( i ) ;
				fprintf( file, "%d\t", f.page_id ) ;
				for ( int j = f.page_frame.size() - 1 ; j >= 0 ; j-- ) {
					fprintf( file, "%d", f.page_frame.at( j ) ) ;
				} // for
				
				if ( f.page_fault == "F" ) {
					fprintf( file, "\t" ) ;
				} // if
				
				fprintf( file, "%s\n", f.page_fault.c_str() ) ;
			} // for
			
			fprintf( file, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n", fault.at( 0 ), replace.at( 0 ), frame_num ) ;
			
			fprintf( file, "--------------LRU-----------------------\n" ) ;
			for ( int i = 0 ; i < lru.size() ; i++ ) {
				Frame f = lru.at( i ) ;
				fprintf( file, "%d\t", f.page_id ) ;
				for ( int j = f.page_frame.size() - 1 ; j >= 0 ; j-- ) {
					fprintf( file, "%d", f.page_frame.at( j ) ) ;
				} // for
				
				if ( f.page_fault == "F" ) {
					fprintf( file, "\t" ) ;
				} // if
				
				fprintf( file, "%s\n", f.page_fault.c_str() ) ;
			} // for
			
			fprintf( file, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n", fault.at( 1 ), replace.at( 1 ), frame_num ) ;
			
			fprintf( file, "--------------Least Frequently Used Page Replacement-----------------------\n" ) ;
			for ( int i = 0 ; i < lfu.size() ; i++ ) {
				Frame f = lfu.at( i ) ;
				fprintf( file, "%d\t", f.page_id ) ;
				for ( int j = f.page_frame.size() - 1 ; j >= 0 ; j-- ) {
					fprintf( file, "%d", f.page_frame.at( j ) ) ;
				} // for
				
				if ( f.page_fault == "F" ) {
					fprintf( file, "\t" ) ;
				} // if
				
				fprintf( file, "%s\n", f.page_fault.c_str() ) ;
			} // for
			
			fprintf( file, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n", fault.at( 2 ), replace.at( 2 ), frame_num ) ;
			
			fprintf( file, "--------------Most Frequently Used Page Replacement -----------------------\n" ) ;
			for ( int i = 0 ; i < mfu.size() ; i++ ) {
				Frame f = mfu.at( i ) ;
				fprintf( file, "%d\t", f.page_id ) ;
				for ( int j = f.page_frame.size() - 1 ; j >= 0 ; j-- ) {
					fprintf( file, "%d", f.page_frame.at( j ) ) ;
				} // for
				
				if ( f.page_fault == "F" ) {
					fprintf( file, "\t" ) ;
				} // if
				
				fprintf( file, "%s\n", f.page_fault.c_str() ) ;
			} // for
			
			fprintf( file, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault.at( 3 ), replace.at( 3 ), frame_num ) ;
			
			fprintf( file, "--------------Least Frequently Used LRU Page Replacement-----------------------\n" ) ;
			for ( int i = 0 ; i < lfu_lru.size() ; i++ ) {
				Frame f = lfu_lru.at( i ) ;
				fprintf( file, "%d\t", f.page_id ) ;
				for ( int j = f.page_frame.size() - 1 ; j >= 0 ; j-- ) {
					fprintf( file, "%d", f.page_frame.at( j ) ) ;
				} // for
				
				if ( f.page_fault == "F" ) {
					fprintf( file, "\t" ) ;
				} // if
				
				fprintf( file, "%s\n", f.page_fault.c_str() ) ;
			} // for
			
			fprintf( file, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n", fault.at( 4 ), replace.at( 4 ), frame_num ) ;
			
			
			fprintf( file, "--------------Most Frequently Used LRU Page Replacement -----------------------\n" ) ;
			for ( int i = 0 ; i < mfu_lru.size() ; i++ ) {
				Frame f = mfu_lru.at( i ) ;
				fprintf( file, "%d\t", f.page_id ) ;
				for ( int j = f.page_frame.size() - 1 ; j >= 0 ; j-- ) {
					fprintf( file, "%d", f.page_frame.at( j ) ) ;
				} // for
				
				if ( f.page_fault == "F" ) {
					fprintf( file, "\t" ) ;
				} // if
				
				fprintf( file, "%s\n", f.page_fault.c_str() ) ;
			} // for
			
			fprintf( file, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n", fault.at( 5 ), replace.at( 5 ), frame_num ) ;
			
		} // else 	
		
		fclose( file ) ;
	} // Output
	
};

	

int main() {
	
	string command = "" ;
	string name = "" ;
	while ( command != "0" ) {
		Mem mem ;
		cout << "請輸入Input檔名(若檔名為Input.txt，請輸入Input)\n:" ;
		cin >> name ;
		mem.Init() ;
		mem.GetInput( name ) ;
		mem.FIFO() ;
		mem.LRU() ;
		mem.LFU() ;
		mem.MFU() ;
		mem.LFU_LRU() ;
		mem.MFU_LRU() ;
		mem.Output( name ) ;
		cout << "請輸入是否要繼續執行(輸入0以外的數字即可繼續)，輸入0即結束程式\n:" ;
		cin >> command ;
	} // while
		
}







