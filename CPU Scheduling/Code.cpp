# include <iostream>
# include <stdio.h>
# include <string.h>
# include <string>
# include <fstream> // 讀檔案
# include <vector> 
# include <time.h>
# include <queue>

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static string fileName = "" ;


struct Data {
	string id ; // 要輸出的ID 
	int pid ; // 數字版的ID 
	int wait_time ; // waitting time
	int done_time ; // turnaround time
	int priority ;
	int arr_time ; // 到達時間 
	int cputime ; // CPU Burst
};

struct Priority_Queue {
	int priority ;
	queue<int> q ;
};

class Scheduling {
public :
	vector<Data> data ;
	vector<Data> fcfs ;
	vector<Data> rr ;
	vector<Data> srtf ;
	vector<Data> pprr ;
	vector<Data> hrrn ;
	int method ;
	int time_slice ;
	string print ;
	
	void Init() {
		data.clear() ;
		fcfs.clear() ;
		rr.clear() ;
		srtf.clear() ;
		pprr.clear() ;
		hrrn.clear() ;
		method = 0 ;
		time_slice = 0 ;
		print = "" ; 
	} // Init()
	
	bool GetInput( string name ) {
		FILE *file = NULL ; // 檔案指標  
		fileName = name ;
		string filename = name + ".txt" ;
		file = fopen( filename.c_str(), "r" ) ; 
		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
		if ( file == NULL ) {
			cout << endl << "檔案開啟失敗!" << endl << endl ; 
		} // if
		else {  
			int num = 0 ;
			int i = 0 ;
			int status = fscanf( file, "%d", &method ) ;
			status = fscanf( file, "%d", &time_slice ) ;
			char temp[10] ;
			for ( int j = 0 ; j < 6 ; j++ ) { // 把字串讀掉 
				status = fscanf( file, "%s", temp ) ;
			} // for
			
			status = fscanf( file, "%d", &num ) ;
			Data d ;
			d.wait_time = 0 ;
			d.done_time = 0 ;
			char n ;
			while ( status != -1 ) {
				d.pid = num ;
				if ( d.pid > 9 ) { // 大於9要轉成英文 
					n = 'A' + d.pid - 10 ;
					d.id = n ;
				} // if
				else {
					n = '0' + d.pid ;
					d.id = n ;
				} // else
				
				fscanf( file, "%d", &d.cputime ) ;
				fscanf( file, "%d", &d.arr_time ) ;
				fscanf( file, "%d", &d.priority ) ;
				data.push_back( d ) ;
				status = fscanf( file, "%d", &num ) ;
			} // while
			
		} // else 	
		
		fclose( file ) ;
	} // Input()
	
	void BubbleSort_pid( vector<Data> &arr ) {
		int i = 0 ;
		int j = 0 ;
		int size = arr.size() ;
		Data temp ;
		while ( size > i ) {
			j = i ;
			while ( j < size - 1 ) {
				if ( arr.at( j ).pid > arr.at( j + 1 ).pid ) {
					temp = arr.at( j ) ;
					arr.at( j ) = arr.at( j + 1 ) ;
					arr.at( j + 1 ) = temp ;
				} // if
				
				j++ ;
			} // while
			
			size-- ;
		} // while
		
	} // BubbleSort_pid()
	
	void BubbleSort_arrtime( vector<Data> & arr ) {
		int i = 0 ;
		int j = 0 ;
		int size = arr.size() ;
		Data temp ;
		while ( size > i ) {
			j = i ;
			while ( j < size - 1 ) {
				if ( arr.at( j ).arr_time > arr.at( j + 1 ).arr_time ) {
					temp = arr.at( j ) ;
					arr.at( j ) = arr.at( j + 1 ) ;
					arr.at( j + 1 ) = temp ;
				} // if
				else if ( arr.at( j ).arr_time == arr.at( j + 1 ).arr_time ) { // 到達時間相同依照PID 
					if ( arr.at( j ).pid > arr.at( j + 1 ).pid ) {
						temp = arr.at( j ) ;
						arr.at( j ) = arr.at( j + 1 ) ;
						arr.at( j + 1 ) = temp ;
					} // if
				} // else if
				
				j++ ;
			} // while
			
			size-- ;
		} // while
		
//		for ( int j = 0 ; j < arr.size() ; j++ ) {
//			Data t = arr.at(j) ;
//			cout << "id : " << t.pid << "    arr : " << t.arr_time << "    cput : " << t.cputime << "    prio : " << t.priority << endl ;
//		} // for
	} // BubbleSort_arrtime()
	
	
	void BubbleSort_priority( vector<Data> & arr ) {
		int i = 0 ;
		int j = 0 ;
		int size = arr.size() ;
		Data temp ;
		while ( size > i ) {
			j = i ;
			while ( j < size - 1 ) {
				if ( arr.at( j ).priority > arr.at( j + 1 ).priority ) {
					temp = arr.at( j ) ;
					arr.at( j ) = arr.at( j + 1 ) ;
					arr.at( j + 1 ) = temp ;
				} // if
				else if ( arr.at( j ).priority == arr.at( j + 1 ).priority ) { // priority相同依照arr
					if ( arr.at( j ).arr_time > arr.at( j + 1 ).arr_time ) {
						temp = arr.at( j ) ;
						arr.at( j ) = arr.at( j + 1 ) ;
						arr.at( j + 1 ) = temp ;
					} // if
					else if ( arr.at( j ).arr_time == arr.at( j + 1 ).arr_time ) { // 到達時間相同依照PID 
						if ( arr.at( j ).pid > arr.at( j + 1 ).pid ) {
							temp = arr.at( j ) ;
							arr.at( j ) = arr.at( j + 1 ) ;
							arr.at( j + 1 ) = temp ;
						} // if
					} // else if
				} // else if
				
				j++ ;
			} // while
			
			size-- ;
		} // while
	} // BubbleSort_priority()
	
	
	void BubbleSort_cputime( vector<Data> & arr ) {
		int i = 0 ;
		int j = 0 ;
		int size = arr.size() ;
		Data temp ;
		while ( size > i ) {
			j = i ;
			while ( j < size - 1 ) {
				if ( arr.at( j ).cputime > arr.at( j + 1 ).cputime ) {
					temp = arr.at( j ) ;
					arr.at( j ) = arr.at( j + 1 ) ;
					arr.at( j + 1 ) = temp ;
				} // if
				else if ( arr.at( j ).cputime == arr.at( j + 1 ).cputime ) { // 到達時間相同依照arr_time 
					if ( arr.at( j ).arr_time > arr.at( j + 1 ).arr_time ) {
						temp = arr.at( j ) ;
						arr.at( j ) = arr.at( j + 1 ) ;
						arr.at( j + 1 ) = temp ;
					} // if
					else if ( arr.at( j ).arr_time == arr.at( j + 1 ).arr_time ) { // 到達時間相同依照PID 
						if ( arr.at( j ).pid > arr.at( j + 1 ).pid ) {
							temp = arr.at( j ) ;
							arr.at( j ) = arr.at( j + 1 ) ;
							arr.at( j + 1 ) = temp ;
						} // if
					} // else if
				} // else if
				
				j++ ;
			} // while
			
			size-- ;
		} // while
	} // BubbleSort_cputime()
	
	void FCFS( bool doAll ) {
		vector<Data> schedule = data ; // 不確定是不是All 先用別的算
		BubbleSort_arrtime( schedule ) ;
		string out = "" ;
		int timer = 0 ;
		for ( int i = 0 ; i < schedule.size() ; i++ ) {
			Data d = schedule.at( i ) ;
			if ( timer <= d.arr_time ) { // 如果到達時沒有人在做 
				for ( int j = timer ; j < d.arr_time ; j++ ) {
					out = out + "-" ;
				} // for
				
				timer = d.arr_time + d.cputime ;
				d.done_time = d.cputime ; // 因為沒有等待
				for ( int j = 0 ; j < d.done_time ; j++ ) {
					out = out + d.id ;
				} // for
				d.wait_time = 0 ; 
			} // if
			else {
				d.wait_time = timer - d.arr_time ; // 真正開始做-到達時間 
				d.done_time = d.wait_time + d.cputime ; // turnaround time = 等待時間+CPU Burst
				for ( int j = 0 ; j < d.cputime ; j++ ) {
					out = out + d.id ;
				} // for
				timer = timer + d.cputime ;
			} // else
			
			schedule.at( i ) = d ; // 存回去 
		} // for
		
		print = print + out + "\n"  ;
		fcfs = schedule ;
		BubbleSort_pid( fcfs ) ;
		if ( !doAll ) {
			print = print + "===========================================================\n" ;
			Output() ;
		} // if
		
	} // FCFS()
	
	void RR( bool doAll ) {
		vector<Data> schedule = data ; // 不確定是不是All 先用別的算
		BubbleSort_arrtime( schedule ) ;
		queue<int> q ;
		string out = "" ;
		int timer = 0 ;
		Data d ;
		bool fin = false ;
		int i = 0 ;
		int pos = 0 ;
		while ( !fin ) {	
			bool push = false ;				
			if ( !q.empty() ) {
				pos = q.front() ;
				d = schedule.at( pos ) ;
				q.pop() ;
				if ( d.cputime > time_slice ) { // 還要排隊 
					d.cputime = d.cputime - time_slice ;
					d.wait_time = d.wait_time + time_slice ; // 等待時間=完成時間-CPUtime 但cputime要減 所以要用他先存起來 
					timer = timer + time_slice ;
					for ( int j = 0 ; j < time_slice ; j++ ) {
						out = out + d.id ;
					} // for
					push = true ;
				} // if
				else {
					timer = timer + d.cputime ;
					for ( int j = 0 ; j < d.cputime ; j++ ) {
						out = out + d.id ;
					} // for
					d.done_time = timer - d.arr_time ;
					d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ; // 等待時間=完成時間-CPUtime 但cputime要減 所以要用他先存起來 
					d.cputime = 0 ;
				} // else
				
				schedule.at( pos ) = d ; // 存回去
			} // if
			else if ( i >= schedule.size() ) {
				fin = true ;
			} // else if


			if ( !fin ) {
				if ( i < schedule.size() ) {
					Data temp = schedule.at( i ) ;
					// 新來的排隊問題 
					if ( timer > temp.arr_time ) { // temp到達了 還有人在做
						while ( i < schedule.size() ) {
							if ( schedule.at( i ).arr_time <= timer ) {
								q.push( i ) ;
							} // if
							else {
								break ;
							} // else 
							
							i++ ;
						} // while
						
						if ( push ) {
							q.push( pos ) ;
						} // if
					} // if
					else if ( timer == temp.arr_time ) { // temp到達了 還有人在做 人家剛好要下車 
						while ( i < schedule.size() ) {
							if ( schedule.at( i ).arr_time == timer ) {
								q.push( i ) ;
							} // if
							else {
								break ;
							} // else 
							
							i++ ;
						} // while
						if ( push ) {
							q.push( pos ) ;
						} // if
					} // else if
					else { // 做完了 但還沒有到達唷 
						if ( push ) {
							q.push( pos ) ;
						} // if
						if ( q.empty() ) { // 有空閒時間 (無人排隊 
							
							for ( int j = timer ; j < temp.arr_time ; j++ ) {
								out = out + "-" ;
							} // for
							timer = temp.arr_time ;
							q.push( i ) ;
							i++ ;
						} // if
					} // else
				} // if
				else {
					if ( push ) {
						q.push( pos ) ;
					} // if
				} // else
			} // if
			
	
		} // while
		
		print = print + out + "\n"  ;
		rr = schedule ;
		BubbleSort_pid( rr ) ;
		if ( !doAll ) {
			print = print + "===========================================================\n" ;
			Output() ;
		} // if
	} // RR()
	
	void FindPos( vector<Data> arr, int &pos, int id ) {
		for ( int i = 0 ; i < arr.size() ; i++ ) {
			if ( arr.at( i ).pid == id ) {
				pos = i ;
				break ;
			} // if
		} // for
	} // FindPos
	
	void SRTF( bool doAll ) {
		vector<Data> schedule = data ; // 不確定是不是All 先用別的算
		vector<Data> q ;
		BubbleSort_arrtime( schedule ) ;
		string out = "" ;
		int timer = 0 ;
		bool fin = false ;
		Data d ;
		Data temp ;
		int i = 0 ;
		while ( !fin ) {
			if ( i < schedule.size() ) {
				temp = schedule.at( i ) ;
				if ( timer >= temp.arr_time ) { // temp到達要排隊 
					while ( i < schedule.size() ) {
						temp = schedule.at( i ) ;
						if ( timer >= temp.arr_time ) {
							q.push_back( temp ) ;
							i++ ;
						} // if
						else {
							break ;
						} // else
					} // while
				} // if
				else { // 有空閒的時間 
					if ( q.empty() ) { // 沒人排隊 
						
						for ( int j = timer ; j < temp.arr_time ; j++ ) {
							out = out + "-" ;
						} // for
						timer = temp.arr_time ;
						q.push_back( temp ) ;
						i++ ;
						if ( i < schedule.size() ) {
							temp = schedule.at( i ) ;
							if ( timer == temp.arr_time ) { // temp到達要排隊 
								while ( i < schedule.size() ) {
									temp = schedule.at( i ) ;
									if ( timer == temp.arr_time ) {
										q.push_back( temp ) ;
										i++ ;
									} // if
									else {
										break ;
									} // else
								} // while
							} // if
						} // if
							
					} // if
				} // else
			} // if
			
			int pos = -1 ;
			if ( !q.empty() ) {
				BubbleSort_cputime( q ) ;
				FindPos( schedule, pos, q.at(0).pid ) ; // 找到到達的人裡面 cputime最小的人的位置
				d = schedule.at( pos ) ;
				if ( i < schedule.size() ) { // 有下個人到達 先做到他到達再判斷 
					temp = schedule.at( i ) ;
					if ( d.cputime > ( temp.arr_time - timer ) ) { // 還沒做完 
						d.cputime = d.cputime - ( temp.arr_time - timer ) ; // 做了開始到停止的時間
						d.wait_time = d.wait_time + ( temp.arr_time - timer ) ; // 紀錄cputime 之後要減掉 
						q.at(0).cputime = d.cputime ;
						for ( int j = timer ; j < temp.arr_time ; j++ ) {
							out = out + d.id ;
						} // for
						timer = temp.arr_time ;
					} // if
					else { // 做完了 
						for ( int j = 0 ; j < d.cputime ; j++ ) {
							out = out + d.id ;
						} // for
						timer = timer + d.cputime ;
						d.done_time = timer - d.arr_time ;
						d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
						d.cputime = 0 ;
						q.erase( q.begin() ) ; // 完成的就去掉 
					} // else	
				} // if
				else { // 大家都排進來了 
					if ( !q.empty() ) { // 直接做完 
						for ( int j = 0 ; j < d.cputime ; j++ ) {
							out = out + d.id ;
						} // for
						timer = timer + d.cputime ;
						d.done_time = timer - d.arr_time ;
						d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
						d.cputime = 0 ;
						q.erase( q.begin() ) ; // 完成的就去掉 
					} // if
				} // else
				
				schedule.at( pos ) = d ;
			} // if
			else {
				fin = true ;
			} // else
				
		} // while
		
		
		print = print + out + "\n"  ;
		srtf = schedule ;
		BubbleSort_pid( srtf ) ;
		if ( !doAll ) {
			print = print + "===========================================================\n" ;
			Output() ;
		} // if

	} // SRTF()
	
	bool NeedRR( vector<Data> arr, int &num ) {
		int priority = arr.at( 0 ).priority ;
		for ( int i = 0 ; i < arr.size() ; i++ ) {
			Data d = arr.at( i ) ;
			if ( d.priority == priority ) {
				num++ ;
			} // if
		} // for
		
		if ( num == 1 ) {
			return false ;
		} // if
		else {
			return true ;
		} // else
	} // NeedRR
	
	bool QueueIsExist( vector<Priority_Queue> arr, int p, int &pos ) {
		for ( int i = 0 ; i < arr.size() ; i++ ) {
			if ( arr.at( i ).priority == p ) {
				pos = i ;
				return true ;
			} // if
		} // for
		
		return false ;
	} // QueueIsExist()
	
	void Priority_Sort( vector<Priority_Queue> & arr ) {
		int i = 0 ;
		int j = 0 ;
		int size = arr.size() ;
		Priority_Queue temp ;
		while ( size > i ) {
			j = i ;
			while ( j < size - 1 ) {
				if ( arr.at( j ).priority > arr.at( j + 1 ).priority ) {
					temp = arr.at( j ) ;
					arr.at( j ) = arr.at( j + 1 ) ;
					arr.at( j + 1 ) = temp ;
				} // if
				
				j++ ;
			} // while
			
			size-- ;
		} // while
	} // Priority_Sort()
	
	void PPRR( bool doAll ) {
		vector<Data> schedule = data ; // 不確定是不是All 先用別的算
		BubbleSort_arrtime( schedule ) ;
		string out = "" ;
		int timer = 0 ;
		bool fin = false ;
		Data d ;
		Data temp ;
		vector<Priority_Queue> q ;
		int i = 0 ;
		int pos = -1 ;
		int index = -1 ;
		Priority_Queue temp_q ;
		int time = 0 ; // RR用 
		bool done = false ; // 判斷RR時有沒有用完time_slice 且該滾的時候還沒滾 
		while ( !fin ) {
			if ( i < schedule.size() ) {
				temp = schedule.at( i ) ;
				if ( timer == temp.arr_time ) { // temp到達要排隊 
					while ( i < schedule.size() ) {
						temp = schedule.at( i ) ;
						if ( timer == temp.arr_time ) {
							if ( !QueueIsExist( q, temp.priority, index ) ) { // 如果這個priority不存在要推進去 
								Priority_Queue temp_q ;
								temp_q.priority = temp.priority ;
								temp_q.q.push( i ) ;
								q.push_back( temp_q ) ;
							} // if
							else {
								q.at( index ).q.push( i ) ;
							} // else
							i++ ;
						} // if
						else {
							break ;
						} // else
					} // while
				} // if
				else { // 有空閒的時間 
					if ( q.empty() ) { // 沒人排隊 
						for ( int j = timer ; j < temp.arr_time ; j++ ) {
							out = out + "-" ;
						} // for
						timer = temp.arr_time ;
						if ( !QueueIsExist( q, temp.priority, pos ) ) { // 如果這個priority不存在要推進去 
							Priority_Queue temp_q ;
							temp_q.priority = temp.priority ;
							temp_q.q.push( i ) ;
							q.push_back( temp_q ) ;
						} // if
						else {
							q.at( pos ).q.push( i ) ;
						} // else

						i++ ;
						if ( i < schedule.size() ) {
							temp = schedule.at( i ) ;
							if ( timer == temp.arr_time ) { // temp到達要排隊 
								while ( i < schedule.size() ) {
									temp = schedule.at( i ) ;
									if ( timer == temp.arr_time ) {
										if ( !QueueIsExist( q, temp.priority, pos ) ) { // 如果這個priority不存在要推進去 
											Priority_Queue temp_q ;
											temp_q.priority = temp.priority ;
											temp_q.q.push( i ) ;
											q.push_back( temp_q ) ;
										} // if
										else {
											q.at( pos ).q.push( i ) ;
										} // else
										i++ ;
									} // if
									else {
										break ;
									} // else
								} // while
							} // if
						} // if	
					} // if
				} // else
			} // if
			
			if ( !q.empty() ) {
				int priority = q.at(0).priority ;
				Priority_Sort( q ) ; // 照優先程度排序 
				temp_q = q.at( 0 ) ; // 最先的queue 
				if ( pos == temp_q.q.front() ) { // 一樣是同一個process做RR(?) 
					if ( done ) { // 他做完了還沒滾下來 
						q.at( 0 ).q.pop() ;
						q.at( 0 ).q.push( pos ) ;
						temp_q = q.at( 0 ) ;
					} // if
						
				} // if
				else {
					if ( done ) { // 前面RR剛好用完的沒pop掉的要pop
						for ( int j = 0 ; j < q.size() ; j++ ) {  
							if ( q.at( j ).priority == priority ) {
								pos = q.at(j).q.front() ;
								q.at( j ).q.pop() ;
								q.at( j ).q.push( pos ) ;
								break ;
							} // if
						} // for
					} // if
						
					time = 0 ;
				} // else
				
				done = false ;
				pos = temp_q.q.front() ; // 最先的queue裡面第一個做的 
				d = schedule.at( pos ) ;
				if ( temp_q.q.size() > 1 ) { // 優先程度相同的有2個以上-->RR 
					if ( i < schedule.size() ) { // 有下個人到達 先做到他到達再判斷 
						temp = schedule.at( i ) ;
						if ( timer + ( time_slice - time ) < temp.arr_time ) { // 用的完time_slice還可以換下一個人 
							if ( d.cputime <= ( time_slice - time ) ) { // 會做完唷唷唷 
								for ( int j = 0 ; j < d.cputime ; j++ ) {
									out = out + d.id ;
								} // for
								timer = timer + d.cputime ;
								d.done_time = timer - d.arr_time ;
								d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
								d.cputime = 0 ;
								q.at( 0 ).q.pop() ; // 做完滾蛋 
							} // if
							else { // 還不夠我做完呢 
								for ( int j = 0 ; j < ( time_slice - time ) ; j++ ) {
									out = out + d.id ;
								} // for
								d.cputime = d.cputime - ( time_slice - time ) ; // 要做多少-做了多少=現在做多少 
								d.wait_time = d.wait_time + ( time_slice - time ) ;
								timer = timer + ( time_slice - time ) ;
								q.at( 0 ).q.pop() ; // 滾去後面排隊 
								q.at( 0 ).q.push( temp_q.q.front() ) ;
							} // else
							
						} // if
						else if ( timer + ( time_slice - time ) == temp.arr_time ) { // 做完剛好有人來 
							if ( d.cputime <= ( time_slice - time ) ) { // 會做完唷唷唷 
								for ( int j = 0 ; j < d.cputime ; j++ ) {
									out = out + d.id ;
								} // for
								timer = timer + d.cputime ;
								d.done_time = timer - d.arr_time ;
								d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
								d.cputime = 0 ;
								q.at( 0 ).q.pop() ; // 做完滾蛋 
							} // if
							else { // 還不夠我做完呢 
								for ( int j = 0 ; j < ( time_slice - time ) ; j++ ) {
									out = out + d.id ;
								} // for
								d.cputime = d.cputime - ( time_slice - time ) ; // 要做多少-做了多少=現在做多少 
								d.wait_time = d.wait_time + ( time_slice - time ) ;
								timer = timer + ( time_slice - time ) ;
								done = true ; // 代表這個人做完了 但是我還沒讓他滾蛋唷 
							} // else
						} // else if
						else { // 還沒用完time_slice就有人來了 可惡 
							if ( d.cputime <= ( temp.arr_time - timer ) ) { // 會做完唷唷唷 
								for ( int j = 0 ; j < d.cputime ; j++ ) {
									out = out + d.id ;
								} // for
								timer = timer + d.cputime ;
								d.done_time = timer - d.arr_time ;
								d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
								d.cputime = 0 ;
								q.at( 0 ).q.pop() ; // 做完滾蛋 
							} // if
							else { // 還不夠我做完呢 
								for ( int j = 0 ; j < ( temp.arr_time - timer ) ; j++ ) {
									out = out + d.id ;
								} // for
								time = temp.arr_time - timer ;
								d.cputime = d.cputime - time ; // 要做多少-做了多少=現在做多少 
								d.wait_time = d.wait_time + time ;
								timer = timer + time ;
								if ( temp_q.priority > temp.priority ) {
									done = true ;
								} // if
							} // else
						} // else
					} // if
					else { // 大家都排進來了 
						if ( !q.empty() ) { // RR 到底 
							
							if ( d.cputime <= ( time_slice - time ) ) { // 做完 
								for ( int j = 0 ; j < d.cputime ; j++ ) {
									out = out + d.id ;
								} // for
								timer = timer + d.cputime ;
								d.done_time = timer - d.arr_time ;
								d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
								d.cputime = 0 ;
								q.at( 0 ).q.pop() ; // 完成的就去掉 
							} // if
							else {
								for ( int j = 0 ; j < ( time_slice - time ) ; j++ ) {
									out = out + d.id ;
								} // for
								timer = timer + ( time_slice - time ) ;
								d.wait_time = d.wait_time + ( time_slice - time ) ;
								d.cputime = d.cputime - ( time_slice - time ) ;
								q.at( 0 ).q.pop() ; // 完成的就去掉 
								q.at( 0 ).q.push( temp_q.q.front() ) ;
							} // else
							
							
						} // if
					} // else
				} // if
				else {
					if ( i < schedule.size() ) { // 有下個人到達 先做到他到達再判斷 
						temp = schedule.at( i ) ;
						if ( d.cputime > ( temp.arr_time - timer ) ) { // 還沒做完 
							time = ( temp.arr_time - timer ) % time_slice ;
							d.cputime = d.cputime - ( temp.arr_time - timer ) ; // 做了開始到停止的時間
							d.wait_time = d.wait_time + ( temp.arr_time - timer ) ; // 紀錄cputime 之後要減掉 
							for ( int j = timer ; j < temp.arr_time ; j++ ) {
								out = out + d.id ;
							} // for
							timer = temp.arr_time ;
						} // if
						else { // 做完了 
							for ( int j = 0 ; j < d.cputime ; j++ ) {
								out = out + d.id ;
							} // for
							timer = timer + d.cputime ;
							d.done_time = timer - d.arr_time ;
							d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
							d.cputime = 0 ;
							q.erase( q.begin() ) ; // 拿掉做完的 
						} // else	
					} // if
					else { // 大家都排進來了 
						if ( !q.empty() ) { // 直接做完 
							for ( int j = 0 ; j < d.cputime ; j++ ) {
								out = out + d.id ;
							} // for
							timer = timer + d.cputime ;
							d.done_time = timer - d.arr_time ;
							d.wait_time = d.done_time - ( d.wait_time + d.cputime ) ;
							d.cputime = 0 ;
							q.erase( q.begin() ) ; // 拿掉做完的
						} // if
						else {
							fin = true ;
						} // else
					} // else
				} // else	
				
				schedule.at( pos ) = d ;
			} // if
			else {
				fin = true ;
			} // else
			
		} // while
		
		
		print = print + out + "\n"  ;
		pprr = schedule ;
		BubbleSort_pid( pprr ) ;
		if ( !doAll ) {
			print = print + "===========================================================\n" ;
			Output() ;
		} // if

	} // PPRR()
	
	void CalRatioSort( vector<Data> & arr, int timer ) {
		vector<float> ratio ;
		float r = 0 ;
		for ( int i = 0 ; i < arr.size() ; i++ ) {
			Data d = arr.at( i ) ;
			r = - ( (float)( timer - d.arr_time + d.cputime ) / (float)( d.cputime ) ) ;
			ratio.push_back( r ) ;
		} // for
		
		int i = 0 ;
		int j = 0 ;
		int size = arr.size() ;
		Data temp ;
		while ( size > i ) {
			j = i ;
			while ( j < size - 1 ) {
				if ( ratio.at( j ) > ratio.at( j + 1 ) ) {
					temp = arr.at( j ) ;
					arr.at( j ) = arr.at( j + 1 ) ;
					arr.at( j + 1 ) = temp ;
					
					r = ratio.at( j ) ;
					ratio.at( j ) = ratio.at( j + 1 ) ;
					ratio.at( j + 1 ) = r ;
				} // if
				else if ( ratio.at( j ) == ratio.at( j + 1 ) ) { // 到達時間相同依照arr_time 
					if ( arr.at( j ).arr_time > arr.at( j + 1 ).arr_time ) {
						temp = arr.at( j ) ;
						arr.at( j ) = arr.at( j + 1 ) ;
						arr.at( j + 1 ) = temp ;
						
						r = ratio.at( j ) ;
						ratio.at( j ) = ratio.at( j + 1 ) ;
						ratio.at( j + 1 ) = r ;
					} // if
					else if ( arr.at( j ).arr_time == arr.at( j + 1 ).arr_time ) { // 到達時間相同依照PID 
						if ( arr.at( j ).pid > arr.at( j + 1 ).pid ) {
							temp = arr.at( j ) ;
							arr.at( j ) = arr.at( j + 1 ) ;
							arr.at( j + 1 ) = temp ;
							
							r = ratio.at( j ) ;
							ratio.at( j ) = ratio.at( j + 1 ) ;
							ratio.at( j + 1 ) = r ;
						} // if
					} // else if
				} // else if
				
				j++ ;
			} // while
			
			size-- ;
		} // while
		
	} // CalRatioSort()
	
	void HRRN( bool doAll ) {
		vector<Data> schedule = data ; // 不確定是不是All 先用別的算
		BubbleSort_arrtime( schedule ) ;
		vector<Data> schedule_ratio ;
		string out = "" ;
		int timer = 0 ;
		int i = 0 ;
		Data d ;
		Data temp ;
		bool fin = false ;
		while ( !fin ) {
			if ( i < schedule.size() ) {
				temp = schedule.at( i ) ;
				if ( timer >= temp.arr_time ) {
					while ( i < schedule.size() ) {
						temp = schedule.at( i ) ;
						if ( timer >= temp.arr_time ) {
							schedule_ratio.push_back( temp ) ;
							i++ ;
						} // if
						else {
							break ;
						} // else
					} // while
					
				} // if
				else { // 有空閒時間 
					if ( schedule_ratio.empty() ) {
						for ( int j = timer ; j < temp.arr_time ; j++ ) {
							out = out + "-" ;
						} // for
						
						timer = temp.arr_time ;
						schedule_ratio.push_back( temp ) ;
						i++ ;
						while ( i < schedule.size() ) {
							temp = schedule.at( i ) ;
							if ( timer >= temp.arr_time ) {
								schedule_ratio.push_back( temp ) ;
								i++ ;
							} // if
							else {
								break ;
							} // else
						} // while
					} // if
						
				} // else
			} // if
			
			int pos = 0 ;
			if ( !schedule_ratio.empty() ) {
				CalRatioSort( schedule_ratio, timer ) ;
				FindPos( schedule, pos, schedule_ratio.at( 0 ).pid ) ; 
				d = schedule.at( pos ) ;
				for ( int j = 0 ; j < d.cputime ; j++ ) {
					out = out + d.id ;
				} // for
				timer = timer + d.cputime ;
				d.done_time = timer - d.arr_time ;
				d.wait_time = d.done_time - d.cputime ;
				d.cputime = 0 ;
				schedule_ratio.erase( schedule_ratio.begin() ) ;
				schedule.at( pos ) = d ;
			} // if
			else {
				fin = true ;
			} // else
			
			
		} // while
		
		print = print + out + "\n"  ;
		hrrn = schedule ;
		BubbleSort_pid( hrrn ) ;

		if ( !doAll ) {
			print = print + "===========================================================\n" ;
			Output() ;
		} // if

	} // HRRN()
	
	void Output() {
		FILE *file = NULL ; // 檔案指標 
		string filename = "out_" + fileName + ".txt" ;
		file = fopen( filename.c_str(), "w" ) ; 
		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
		if ( file == NULL ) {
			cout << endl << "檔案開啟失敗!" << endl << endl ; 
		} // if
		else {  
			Data d ;
			fprintf( file, "%s\n", print.c_str() ) ;
			fprintf( file, "waiting\n" ) ;
			if ( method == 1 ) {
				fprintf( file, "ID	FCFS\n===========================================================\n" ) ;
				for ( int i = 0 ; i < fcfs.size() ; i++ ) {
					d = fcfs.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.wait_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				fprintf( file, "Turnaround Time\n" ) ;
				fprintf( file, "ID	FCFS\n===========================================================\n" ) ;
				for ( int i = 0 ; i < fcfs.size() ; i++ ) {
					d = fcfs.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.done_time ) ;
				} // for
			} // if
			else if ( method == 2 ) {
				fprintf( file, "ID	RR\n===========================================================\n" ) ;
				for ( int i = 0 ; i < rr.size() ; i++ ) {
					d = rr.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.wait_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				fprintf( file, "Turnaround Time\n" ) ;
				fprintf( file, "ID	RR\n===========================================================\n" ) ;
				for ( int i = 0 ; i < rr.size() ; i++ ) {
					d = rr.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.done_time ) ;
				} // for
			} // else if
			else if ( method == 3 ) {
				fprintf( file, "ID	SRTF\n===========================================================\n" ) ;
				for ( int i = 0 ; i < srtf.size() ; i++ ) {
					d = srtf.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.wait_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				fprintf( file, "Turnaround Time\n" ) ;
				fprintf( file, "ID	SRTF\n===========================================================\n" ) ;
				for ( int i = 0 ; i < srtf.size() ; i++ ) {
					d = srtf.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.done_time ) ;
				} // for
			} // else if
			else if ( method == 4 ) {
				fprintf( file, "ID	PPRR\n===========================================================\n" ) ;
				for ( int i = 0 ; i < pprr.size() ; i++ ) {
					d = pprr.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.wait_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				fprintf( file, "Turnaround Time\n" ) ;
				fprintf( file, "ID	PPRR\n===========================================================\n" ) ;
				for ( int i = 0 ; i < pprr.size() ; i++ ) {
					d = pprr.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.done_time ) ;
				} // for
			} // else if
			else if ( method == 5 ) {
				fprintf( file, "ID	HRRN\n===========================================================\n" ) ;
				for ( int i = 0 ; i < hrrn.size() ; i++ ) {
					d = hrrn.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.wait_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				fprintf( file, "Turnaround Time\n" ) ;
				fprintf( file, "ID	HRRN\n===========================================================\n" ) ;
				for ( int i = 0 ; i < hrrn.size() ; i++ ) {
					d = hrrn.at(i) ;
					fprintf( file, "%d\t%d\n", d.pid, d.done_time ) ;
				} // for
			} // else if
			else if ( method == 6 ) {			
				fprintf( file, "ID	FCFS	RR	SRTF	PPRR	HRRN\n===========================================================\n" ) ;
				for ( int i = 0 ; i < fcfs.size() ; i++ ) {
					d = fcfs.at(i) ;
					fprintf( file, "%d\t%d\t", d.pid, d.wait_time ) ;
					d = rr.at(i) ;
					fprintf( file, "%d\t", d.wait_time ) ;
					d = srtf.at(i) ;
					fprintf( file, "%d\t", d.wait_time ) ;
					d = pprr.at(i) ;
					fprintf( file, "%d\t", d.wait_time ) ;
					d = hrrn.at(i) ;
					fprintf( file, "%d\n", d.wait_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				fprintf( file, "Turnaround Time\n" ) ;
				fprintf( file, "ID	FCFS	RR	SRTF	PPRR	HRRN\n===========================================================\n" ) ;
				for ( int i = 0 ; i < fcfs.size() ; i++ ) {
					d = fcfs.at(i) ;
					fprintf( file, "%d\t%d\t", d.pid, d.done_time ) ;
					d = rr.at(i) ;
					fprintf( file, "%d\t", d.done_time ) ;
					d = srtf.at(i) ;
					fprintf( file, "%d\t", d.done_time ) ;
					d = pprr.at(i) ;
					fprintf( file, "%d\t", d.done_time ) ;
					d = hrrn.at(i) ;
					fprintf( file, "%d\n", d.done_time ) ;
				} // for
				
				fprintf( file, "===========================================================\n\n" ) ;
				
			} // else if
		} // else 	
		
		fclose( file ) ;
	} // Output
	

	
};



int main() {
	string name = "" ;
	string num = "" ;
	while ( num != "0" ) {
		Scheduling s ;
		s.Init() ;
		cout << "請輸入Input檔名(若檔名為Input.txt，請輸入Input)\n:" ;
		cin >> name ;
		s.GetInput( name ) ;
		if ( s.method == 1 ) {
			s.print = "FCFS\n" ;
			s.FCFS( false ) ;
		} // if
		else if ( s.method == 2 ) {
			s.print = "RR\n" ;
			s.RR( false ) ;
		} // else if
		else if ( s.method == 3 ) {
			s.print = "SRTF\n" ;
			s.SRTF( false ) ;
		} // else if
		else if ( s.method == 4 ) {
			s.print = "PPRR\n" ;
			s.PPRR( false ) ;
		} // else if
		else if ( s.method == 5 ) {
			s.print = "HRRN\n" ;
			s.HRRN( false ) ;
		} // else if
		else if ( s.method == 6 ) {
			s.print = "All\n" ;
			s.print = s.print + "==        FCFS==\n" ;
			s.FCFS( true ) ;
			s.print = s.print + "==          RR==\n" ;
			s.RR( true ) ;
			s.print = s.print + "==        SRTF==\n" ;
			s.SRTF( true ) ;
			s.print = s.print + "==        PPRR==\n" ;
			s.PPRR( true ) ;
			s.print = s.print + "==        HRRN==\n" ;
			s.HRRN( true ) ;
			s.print = s.print + "===========================================================\n" ;
			s.Output() ;
		} // else if
		
		cout << "請輸入是否要繼續執行(輸入0以外的數字即可繼續)，輸入0即結束程式\n:" ;
		cin >> num ;

	} // while
		
	
	
}

