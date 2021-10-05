#include <iostream>
#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h>
#include <string>
#include <fstream> // 讀檔案 
#include <vector>
#include <sstream> // 10轉8,16 
using namespace std; 
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static vector<string> source ;
static string fileName = "" ;


class LexicalAnalysis{
	private:
		vector<string> table1 ;
		vector<string> table2 ;
		vector<string> table3 ;
		vector<string> table4 ;
		string table5[100] ;
		string table6[100] ;
		string table7[100] ;
	public:
	
	void Copy( vector<string> &t1, vector<string> &t2, vector<string> &t3, vector<string> &t4, string t5[], string t6[], string t7[] ) {
		t1 = table1 ;
		t2 = table2 ;
		t3 = table3 ;
		t4 = table4 ;
		int i = 0 ;
		for ( i = 0 ; i < 100 ; i++ )
			t5[i] = table5[i] ;
		for ( i = 0 ; i < 100 ; i++ )
			t6[i] = table6[i] ;
		for ( i = 0 ; i < 100 ; i++ )
			t7[i] = table7[i] ;
	} // Copy
	
	void Init() {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table5[i] = "" ;
		} // for
		for ( int j = 0 ; j < 100 ; j++ ) {
			table6[j] = "" ;
		} // for
		for ( int k = 0 ; k < 100 ; k++ ) {
			table7[k] = "" ;
		} // for
	} // Init()
	
	bool InputTable() {
		ReadTable( "1" ) ;
		ReadTable( "2" ) ;
		ReadTable( "3" ) ;
		ReadTable( "4" ) ;
	} // InputTable
	
	void ReadTable( string num ) {
		FILE *file = NULL ; // 檔案指標 
		string name = "table" + num ;
		string filename = name + ".table" ;
		char number = name[name.length()-1] ;
		if ( number == '1' ) {
			table1.clear() ;
		} // if
		else if ( number == '2' ) {
			table2.clear() ;
		} // else if
		else if ( number == '3' ) {
			table3.clear() ;
		} // else if
		else if ( number == '4' ) {
			table4.clear() ;
		} // else if
		file = fopen( filename.c_str(), "r" ) ; 
		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
		if ( file == NULL ) {
			cout << endl << "檔案開啟失敗!" << endl << endl ; 
		} // if
		else {  
			char instr[10] ;
			int num = fscanf( file, "%s", instr ) ;
			while ( num != -1 ) {
				if ( number == '1' ) {
					table1.push_back( instr ) ;
				} // if
				else if ( number == '2' ) {
					table2.push_back( instr ) ;
				} // else if
				else if ( number == '3' ) {
					table3.push_back( instr ) ;
				} // else if
				else if ( number == '4' ) {
					table4.push_back( instr ) ;
				} // else if
				
				
				num = fscanf( file, "%s", instr ) ;
			} // while
				
		} // else 	
		
		fclose( file ) ;
				
	} // ReadTable()
	
	void WriteFile( string line ) { 
		string name = fileName + "_output.txt" ;
		FILE *out ;
		out = fopen( name.c_str(), "a" ) ;
		if ( out == NULL ) {
			cout << "檔案寫入失敗!" << endl << endl ;
		} // if
		else {
			char temp[line.length()+1] ;
			temp[0] = '\0' ;
			strcpy( temp, line.c_str() ) ;
			fprintf( out, "%s", temp ) ;
		} // else

		fclose( out ) ;
	} // WriteFile
	
	bool IsWhiteSpace( string article, int index ) {
		if ( article[index] == ' ' || article[index] == '	' || article[index] == '\n' ) {
			return true ;
		} // if
		else return false ;
	} // IsWhiteSpace
	
	int PositionOfWord( string article, int index ) {
		while ( index < article.size() ) {
			if ( IsWhiteSpace( article, index ) ) {
				index++ ;
			} // if
			else {
				return index ;
			} // else
		} // while
		
		return -1 ; // 找到換行了-->沒東西 
	} // PositionOfWord()
	
	bool IsDelimiter( char word ) {
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( table4.at(i)[0] == word ) {
				return true ;
			} // if
		} // for
		
		return false ;
	} // IsDelimiter
	
	string CatchWord( string article, int &index ) {
		string temp = "" ;
		while ( !IsWhiteSpace( article, index ) && !IsDelimiter( article[index] ) && index < article.length() ) {
			temp = temp + article[index] ;
			index++ ;		
		} // while
		
		if ( temp == "" && index < article.length() && IsDelimiter( article[index] ) ) {
			temp = article[index] ; 
			index++ ;
		} // if
		return temp ;
			
	} // CatchWord()
	
	void FindTable4( string temp ) {
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( temp == table4.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(4," + t + ")" ;
				WriteFile( word ) ;
			} // if
		} // for
	} // FindTable4
	
	bool FindTable1( string temp ) { // a 97 A 65
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //大寫的字轉小寫 
			name1[j] = temp[j] ;
			if ( temp[j] >= 'A' && temp[j] <= 'Z' ) {
				name1[j] = temp[j] + 32 ;
			} // if	
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table1.size() ; i++ ) { // 小寫 
			if ( temp == table1.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(1," + t + ")" ;
				WriteFile( word ) ;
				return true ;
			} // if
			else if ( name == table1.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(1," + t1 + ")" ;
				WriteFile( token ) ;
				return true ;
			} // else if
		} // for
		
		return false ;
	} // FindTable1
	
	bool FindTable2( string temp ) {
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //小寫的字轉大寫  
			name1[j] = temp[j] ;
			if ( temp[j] >= 'a' && temp[j] <= 'z' ) {
				name1[j] = temp[j] - 32 ;
			} // if
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table2.size() ; i++ ) { // 大寫 
			if ( temp == table2.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(2," + t + ")" ;
				WriteFile( word ) ;
				return true ;
			} // if
			else if ( name == table2.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(2," + t1 + ")" ;
				WriteFile( token ) ;
				return true ;
			} // else if 
		} // for
		
		return false ;
	} // FindTable2
	
	bool FindTable3( string temp ) { // X的問題尚未解決!!!!!!!!!!!! 
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //小寫的字轉大寫   
			name1[j] = temp[j] ;
			if ( temp[j] >= 'a' && temp[j] <= 'z' ) {
				name1[j] = temp[j] - 32 ;
			} // if
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table3.size() ; i++ ) {  
			if ( temp == table3.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(3," + t + ")" ;
				WriteFile( word ) ;
				return true ;
			} // if
			else if ( name == table3.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(3," + t1 + ")" ;
				WriteFile( token ) ;
				return true ;
			} // else if 
		} // for
		
		return false ;
	} // FindTable3
	
	void MakeHash( string temp ) {
		bool out = false ;
		for ( int i = 0 ; i < temp.length() && !out ; i++ ) {
			if ( temp[i] >= '0' && temp[i] <= '9' ) {
				;
			} // if
			else { // 有非數字的!! symbol!! 
				PutInTable567( temp, table5, "5" ) ;
				out = true ;
			} // else
		} // for
		
		if ( !out ) { // 是數字 
			PutInTable567( temp, table6, "6" ) ;
		} // if
	} // MakeHash
	
	void CheckTable( string temp, string article, int &index ) {
		if ( temp.length() == 1 && IsDelimiter( temp[0] ) ) { // temp只有一個字-->第一個字是delimiter 
			FindTable4( temp ) ; 
			if ( temp == "." ) {
				index = article.length() ; // 註解全部都不要了! 
			} // if
		} // if
		else {
			if ( !FindTable1( temp ) ) {
				if ( !FindTable2( temp ) ) {
					if ( article[index] != '\'' && !FindTable3( temp ) ) { // 不是table1 2 3 4 要自己建立 
						MakeHash( temp ) ;
					} // if
					else if ( article[index] == '\'' && ( temp == "X" || temp == "x" || temp == "c" || temp == "C" ) ) { // 可能是C'EOF' X'F1' 
						PutSpec( temp, article, index ) ;
					} // else if
					else if ( article[index] == '\'' ) { // 可能是Xsd'155 
						MakeHash( temp ) ;
					} // else
				} // if
			} // if
		} // else
	} // CheckTable
	
	void PutInTable567( string temp, string table[100], string tableNum ) {
		int sum = 0 ;
		for ( int i = 0 ; i < temp.length() ; i++ ) {
			sum = sum + temp[i] ;
		} // for
		bool done = false ;
		int index = sum % 100 ;
		while ( table[index] != "" && !done ) {
			if ( index == 99 ) {
				index = 0 ;
			} // if
			else if ( temp == table[index] ) {
				done = true ;
			} // else if
			else {
				index++ ;
			} // else 
		} // while
		
		table[index] = temp ;
		char number[3] ;
		itoa( index, number, 10 ) ;
		string t1 = number ;
		string token = "(" + tableNum + "," + t1 + ")" ;
		WriteFile( token ) ;
	} // PutInTable567 
	
	string CatchString( string temp, int &index ) {
		string t = "" ;
		string null = "" ;
		for ( int i = index ; i < temp.length() ; i++ ) {
			if ( temp[i] == '\'' ) {
				index = i ;
				return t ;
			} // if
		    else {
				t = t + temp[i] ;
			} // else
		} // for
		
		return null ;
	} // CatchString
	
	string CatchInt( string number, int &index ) {
		string temp = "" ;
		string null = "" ;
		for ( int i = index ; i < number.length() ; i++ ) {
			if ( number[i] == ' ' || number[i] == '\t' ) {
				;
			} // if
			else if ( number[i] == '\'' ) {
				index = i ;
				return temp ;
			} // else if
			else {
				temp = temp + number[i] ;
			} // else
		} // for
		
		return null ;
	} // CatchInt
	
	void PutSpec( string temp, string article, int &index ) {
		if ( temp == "X" || temp == "C" || temp == "c" || temp == "x" ) {
			int i = index ;
			string word = CatchWord( article, i ) ;
			int re = i ; // C'或X'後面的index位置 
			int position = i ;
			if ( word == "'" ) {
				string number = CatchInt( article, i ) ;
				string eof = CatchString( article, position ) ;
				bool out = false ;
				if ( ( temp == "X" || temp == "x" ) ) { // 為X'F1'的形式-->確認是不是16進制數字! 
					for ( int j = 0 ; j < number.length() && !out ; j++ ) {
						if ( ( number[j] <= 'F' && number[j] >= 'A' ) || ( number[j] <= 'f' && number[j] >= 'a' ) ) {
							;
						} // if
						else if ( number[j] <= '9' && number[j] >= '0' ) {
							;
						} // else if
						else { // 裡面放的不是數字!!! -->那就是symbol-->字串有空格要切開來!!! 
							FindTable3( temp ) ; // 放X 
							FindTable4( word ) ; 
							string str = CatchWord( article, re ) ;
							bool label = false ;
							for ( int b = 0 ; b < str.length() ; b++ ) {
								if ( str[b] < '0' || str[b] > '9' ) {
									label = true ;
								} // if
							} // for
							if ( label ) {
								PutInTable567( str, table5, "5" ) ;
							} // if
							else {
								PutInTable567( str, table6, "6" ) ;
							} // else
							index = re ;
							out = true ;
						} // else
					} // for
					
					if ( !out ) { //是數字(吧) 
						if ( number != "" ) { // X'F1'的情況 
							FindTable4( word ) ;
							for ( int a = 0 ; a < number.length() ; a++ ) {
								if ( number[a] >= 'a' && number[a] <= 'f' ) {
									number[a] = number[a] - 32 ;
								} // if
							} // for
							PutInTable567( number, table6, "6" ) ;
						} // if
						else { // 只有一個'的情況或X'' 
							string t = CatchWord( article, i ) ; // X'之後的字
							if ( t == "'" ) { // X''的情況 
								FindTable4( word ) ;
								FindTable4( t ) ;
							}  // if
							else if ( t == "" ) { // X'的情況 
								FindTable3( temp ) ; // 放X 
								FindTable4( word ) ;
							} // else if
							else { // X'asdf || X'123
								FindTable3( temp ) ; // 放X 
								FindTable4( word ) ;
								bool num = true ;
								for ( int k = 0 ; k < t.length() ; k++ ) { // 判斷是symbol還是數字 
									if ( t[k] < '0' || t[k] > '9' ) {
										num = false ;
									} // if
								} // for
								if ( num ) {
									PutInTable567( t, table6, "6" ) ;
								} // if
								else {
									PutInTable567( t, table5, "5" ) ;
								} // else
							} // else
						} // else

						index = i ;
					} // if
				} // if
				else if ( ( temp == "C" || temp == "c" ) ) { 
					if ( eof != "" ) { // 為C'6516as'的情況 
						FindTable4( word ) ;
						PutInTable567( eof, table7, "7" ) ;
					} // if
					else {
						string w = CatchWord( article, position ) ;
						if ( w == "'" ) { // C''的情況 
							FindTable4( word ) ;
							FindTable4( w ) ;
						} // if
						else if ( w == "" ) { // C'的情況 
							PutInTable567( temp, table5, "5" ) ;
							FindTable4( word ) ;
						} // else if
						else { // C'asd156的情況 
							PutInTable567( temp, table5, "5" ) ;
							FindTable4( word ) ;
							bool symbol = false ;
							for ( int j = 0 ; j < w.length() ; j++ ) {
								if ( w[j] < '0' || w[j] > '9' ) {
									symbol = true ;
								} // if
							} // for
							if ( symbol ) {
								PutInTable567( w, table5, "5" ) ;
							} // if
							else {
								PutInTable567( w, table6, "6" ) ;
							} // else
						} // else
					} // else
					
					index = position ;
				} // else if
			} // if
			else {
				PutInTable567( temp, table5, "5" ) ;
			} // else
		} // if
	} // PutSpec
	
	void CutToken() {
		string temp = "" ;
		string article = "" ;
		int line = 0 ;
		while ( line < source.size() ) {
			article = source.at(line) ; // 第line行內容
			WriteFile( article ) ;
			int index = 0 ;
			temp = "" ;
			index = PositionOfWord( article, index ) ; // 先跳過前面的空格
			while ( index != -1 ) { // 此行還沒抓完 
				temp = CatchWord( article, index ) ; // index為temp後一個字 
				// 抓出一個完整的字(在white space或delimiter前的word) index為whitespace或delimiter的位置
				if ( temp != "" ) { // 有東西! 
					CheckTable( temp, article, index ) ;
				} // if
				index = PositionOfWord( article, index ) ; // 找這行下一個字的位置
				 
			} // while
			
			WriteFile( "\n" ) ;
			line++ ; // 換行 

			
			
		}  // while
		WriteTable567() ;
	} // CutToken()
	
	void WriteTable567() {
		string name = "Table5.table" ;
		int i = 0 ;
		FILE *out ;
		out = fopen( name.c_str(), "w" ) ;
		if ( out == NULL ) {
			cout << "檔案寫入失敗!" << endl << endl ;
		} // if
		else {
			i = 0 ;
			while ( i < 100 ) {
				fprintf( out, "%s\n", table5[i].c_str() ) ;
				i++ ;
			} // while
		} // else

		fclose( out ) ;
		
		out = fopen( "Table6.table", "w" ) ;
		if ( out == NULL ) {
			cout << "檔案寫入失敗!" << endl << endl ;
		} // if
		else {
			i = 0 ;
			while ( i < 100 ) {
				fprintf( out, "%s\n", table6[i].c_str() ) ;
				i++ ;
			} // while
		} // else

		fclose( out ) ;
		
		out = fopen( "Table7.table", "w" ) ;
		if ( out == NULL ) {
			cout << "檔案寫入失敗!" << endl << endl ;
		} // if
		else {
			i = 0 ;
			while ( i < 100 ) {
				fprintf( out, "%s\n", table7[i].c_str() ) ;
				i++ ;
			} // while
		} // else

		fclose( out ) ;
	} // WriteTable567
	
}; // LexicalAnalysis()


bool ReadInputSource() {
	source.clear() ;
	FILE *file = NULL ; // 檔案指標 
	fileName = "" ;
	string name = "" ;
	cout << "請輸入檔案名稱:\n(ex:若讀入檔案為input.txt，請輸入input)\n提醒!:若要重複執行相同的input檔案，資料將會接著寫在同個output檔案下方，故請先刪除之前的output檔!\n:" ;
	cin >> name ;
	fileName = name ;
	name =  name + ".txt" ;
	file = fopen( name.c_str(), "r" ) ; 
	  // 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
	if ( file == NULL ) {
		cout << endl << "檔案開啟失敗!" << endl << endl ; 
		fclose( file ) ;
		return false ;
	} // if
	else { 
		char temp = '\0' ;
		int num = fscanf( file,"%c", &temp );
		while ( num != -1 ) {
			string line = "" ;
			int number = 9999 ;
			while ( temp != '\n' && number != -1 ) {
				line = line + temp ;
				number = fscanf( file,"%c", &temp );
			} // while
			
			if ( temp != '\n' ) {
				line = line + "\n" ;
			} // if
			else {
				line = line + temp ;
			} // else
			source.push_back( line ) ;
			if ( number != -1 ) {
				num = fscanf( file,"%c", &temp ) ;
			} // if
			else {
				num = -1 ;
			} // else
		} // while
					
	} // else 	
	
	fclose( file ) ;
	return true ;			
} // ReadInputSource()


void Lexical_Analysis( LexicalAnalysis &lex ) {
	char act = '\0' ;
	lex.Init() ;
	if ( ReadInputSource() ) {
		lex.InputTable() ;
		lex.CutToken() ;
		cout << endl << "切好token囉!" << endl ;
	} // if

} // Lexical_Analysis()

struct Format {
	string opcode ;
	int type ; // format
	int opnum ; // operand Number
	string optype ; // operand用什麼 
};

struct Symbol {
	string name ;
	string address ;
};

struct Table {
	string name ;
	int table ;
	int position ;
}; 

struct SIC {
	Table symbol ; 
	Table instr ;
	Table op1 ;
	Table op2 ;
	bool X = false ;
};

struct OutputXE {
	string loc ;
	SIC instruction ; 
	string allInstr ; // 完整的指令 
	string code ;
	int PC = -1 ;
	bool error = false ;
	bool type4 = false ;
}; 

struct Output {
	string loc ;
	SIC instruction ; // 切好的指令token 
	string allInstr ; // 完整的指令 
	string code ;
	bool error = false ;
};

class SyntaxAnalysis {
	private :
		vector<Output> output ;
		vector<OutputXE> outXE ;
		vector<string> input ;
		vector<string> table1 ;
		vector<string> table2 ;
		vector<string> table3 ;
		vector<string> table4 ;
		vector<Symbol> symtab ;
		int baseReg = 0 ;
		string table5[100] ;
		string table6[100] ;
		string table7[100] ;
		Format form[60] ;
		string literal[100] ; 
	public :
		void ReadFile() {
			string name = fileName + "_output.txt" ;
			FILE *file = NULL ; // 檔案指標 
			file = fopen( name.c_str(), "r" ) ; 
	  		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
			if ( file == NULL ) {
				cout << endl << "檔案開啟失敗!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				string temp = "" ;
				char ch ;
				int num = fscanf( file, "%c", &ch );
				while ( num != -1 ) {
					if ( ch == '\n' ) {
						input.push_back(temp) ;
						temp = "" ;
					} // if
					else {
						temp = temp + ch ;
					} // else
		
					if ( num != -1 )
						num = fscanf( file, "%c", &ch );
				} // while
			} // else 	
	
			fclose( file ) ;
			
		} // ReadFile
		
		void ReadFormat() {
			string name = "optable.table" ;
			FILE *file = NULL ; // 檔案指標 
			file = fopen( name.c_str(), "r" ) ; 
	  		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
			if ( file == NULL ) {
				cout << endl << "檔案開啟失敗!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				char temp[5] ;
				int num = fscanf( file, "%s", temp ) ;
				int i = 0 ;
				while ( num != -1 && i < 59 ) { 
					form[i].opcode = temp ;
					fscanf( file, "%d", &form[i].type ) ;
					fscanf( file, "%d", &form[i].opnum ) ;
					fscanf( file, "%s", temp ) ;
					form[i].optype = temp ;
					i++ ;
					num = fscanf( file, "%s", temp ) ;
				} // while
			} // else 	
	
			fclose( file ) ;
		} // ReadFormat
		
		void Copy( LexicalAnalysis lex ) {
			lex.Copy( table1, table2, table3, table4, table5, table6, table7 ) ;
		} // Copy
		
		void FindPosition( string format, int &table, int &position ) {
			int i = 3 ; // 第一個一定是( 第二個是tableNUM 第三個是, 
			table = format[1] - '0' ;
			string temp = "" ;
			while ( i < format.length() ) {
				if ( format[i] == ')' ) {
					position = atoi(temp.c_str()) ;
				} // if
				else {
					temp = temp + format[i] ;
				} // else
				i++ ;
			} // while
		} // FindPosition
		
		bool NextIsNothing( int j, int table[], int position[], int num ) { // 確認後面是不是沒東西了 
			j++ ;
			if ( j == num ) { // 沒東西啦! 
				return true ;
			} // if
			else if ( table[j] == 4 && position[j] == 10 ) { // 註解(4,10) 
				return true ;
			} // else if
			else { // 還有東西! 
				return false ;
			} // else
		} // NextIsNothing()
		
		bool TypeMofTable4( int j, int table[], int position[], int num, int type ) { // 將opnum = 1 optype = m的重複程式碼寫在這 
			j++ ;
			if ( type == 1 && j < num && ( table[j] == 6 || table[j] == 5 ) ) { // 可接數字或symbol # 
				j++ ;
				if ( j == num ) { // 沒東西啦! 
					return true ;
				} // if
				else if ( table[j] == 4 && position[j] == 10 ) { // 註解(4,10) 
					return true ;
				} // else if
				else { // 還有東西! 
					return false ;
				} // else
			} // if
			else { // 其他東西! 或沒東西了 -->literal特有情況 
				if ( type == 3 ) {
					if ( j < num && table[j] == 4 && position[j] == 9 ) { // C'EOF' || X'F1' =
						j++ ;
						if ( j < num && ( table[j] == 6 || table[j] == 7 ) ) {
							return true ;
						} // if
						else {
							return false ;
						} // else
					} // if
					else {
						return false ;
					} // else
				} // if
				else if ( type == 2 && j < num && table[j] == 5 ) { // symbol @
					j++ ;
					if ( j == num ) { // 沒東西啦! 
						return true ;
					} // if
					else if ( table[j] == 4 && position[j] == 10 ) { // 註解(4,10) 
						return true ;
					} // else if
					else { // 還有東西! 
						return false ;
					} // else
				} // else if
				else {
					return false ;
				} // else	
			} // else
			
		} // TypeMofTable4
		
		bool CheckTypeSeq( int table[], int position[], int num ) { // 檢查table順序對嗎 
			int i = 0 ;
			int j = 0 ;
			int index = 0 ;
			int opnum = 0 ;
			string optype = "" ;
			Format temp ;
			while ( i < num ) {
				if ( table[i] == 1 ) { // 指令 
					index = position[i] - 1 ; // table1的position從1開始 
					temp = form[index] ;
					opnum = temp.opnum ;
					optype = temp.optype ;
					j = i + 1 ;
					if ( opnum == 0 ) { // 無operand 
						if ( j == num ) { // 那指令應為最後一個(不然就是有註解!) 
							return true ;
						} // if
						else if ( table[j] == 4 && position[j] == 10 ) { // 註解(4,10) 
							return true ;
						} // else if
					} // if
					else if ( j == num ) { // 下一個應該要有東西! opnum != 0
						return false ;
					} // else if
					else if ( opnum == 1 ) { // 應該只有一個operand 
						if ( optype == "r" ) { // 為reg	
							if ( table[j] == 3 ) { // 是reg! 
								return NextIsNothing( j, table, position, num ) ;
							} // if
							else { // 後面不是reg! 
								return false ;
							} // else
						} // if
						else if ( optype == "m" ) { // 後面接symbol 或 # 或 @ 或 literal 或 address 
							if ( table[j] == 5 ) { // 後面接symbol 
								j++ ;
								if ( j < num && ( table[j] == 4 && position[j] == 1 ) ) { // ,X 的情況-->index 
									j++ ;
									if ( j < num && ( table[j] == 3 && position[j] == 2 ) ) { // X
										return NextIsNothing( j, table, position, num ) ;
									} // if
									else { // 不是X!! 
										return false ;
									} // else
								} // if
								else {
									return NextIsNothing( j-1, table, position, num ) ;
								} // else
							} // if
							else if ( table[j] == 4 && position[j] == 12 ) { // #
								return TypeMofTable4( j, table, position, num, 1 ) ;
							} // else if
							else if ( table[j] == 4 && position[j] == 13 ) { // @
								return TypeMofTable4( j, table, position, num, 2 ) ;
							} // else if
							else if ( table[j] == 4 && position[j] == 11 ) { // =
								return TypeMofTable4( j, table, position, num, 3 ) ;
							} // else if
							else if ( table[j] == 6 ) { // address
								return NextIsNothing( j, table, position, num ) ;
							} // else if
							else { // 後面接的不對! 
								return false ;
							} // else
						} // else if
						else { // 只剩下n款 
							if ( table[j] == 6 ) { // 直接接數字(吧) 
								return true ;
							} // if
							else { // 不是數字! 
								return false ;
							} // else
						} // else
					} // else if
					else { // 最多兩個operand!  
						if ( optype == "rr" ) { // 皆為reg 
							if ( table[j] == 3 ) { // 第一個是reg 
								j++ ;
								if ( j == num ) { // 只有一個operand 
									return false ;
								} // if
								else if ( table[j] == 4 && position[j] == 1 ) { // 下一個是, 
									j++ ;
									if ( j == num ) { // 應該還有 
										return false ;
									} // if
									else if ( table[j] == 3 ) { // 有2個reg了確認後面還有沒有 
										return NextIsNothing( j, table, position, num ) ;
									} // else if
									else { // 接的不是reg 
										return false ;
									} // else
								} // else if
								else { // 接的不是,
									return false ;
								} // else
							} // if
							else { // 不是reg! 
								return false ;
							} // else
						} // if
						else { // 只剩rn的type 
							if ( table[j] == 3 ) { // 第一個是reg 
								j++ ;
								if ( j == num ) { // 只有一個! 
									return false ;
								} // if
								else if ( table[j] == 4 && position[j] == 1 ) { // ,
									j++ ;
									if ( j == num ) {
										return false ;
									} // if
									else if ( table[j] == 6 ) { // reg,n
										return NextIsNothing( j, table, position, num ) ;
									} // else if
									else { // 接的不是數字! 
										return false ;
									} // else
								} // else if
								else { // 接的不是, 
									return false ;
								} // else
							} // if
						} // else
					} // else
				} // if
				else if ( table[i] == 2 ) { // 假指令! 
					j = i + 1 ;
					if ( position[i] == 1 ) { // START
						if ( j == num ) {
							return false ;
						} // if
						else if ( table[j] == 6 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else { // 接的不是數字 
							return false ;
						} // else
					} // if
					else if ( position[i] == 2 ) { // END
						if ( j == num ) { // 後面可以沒東西 
							return true ;
						} // if
						else if ( table[j] == 5 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else { // 接的不是symbol 
							return false ;
						} // else
					} // else if
					else if ( position[i] == 9 ) { // LTORG
						return NextIsNothing( i, table, position, num ) ;
					} // else if
					else if ( position[i] == 8 ) { // BASE 後面可以接symbol或decNum 
						if ( j == num ) { 
							return false ;
						} // if
						else if ( table[j] == 5 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else if ( table[j] == 6 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else {
							return false ;
						} // else
					}  // else if
					else if ( position[i] == 5 || position[i] == 6 ) { // RESW RESB
						if ( j == num ) {
							return false ;
						} // if
						else if ( table[j] == 6 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else {
							return false ;
						} // else
					} // else if
					else if ( position[i] == 3 || position[i] == 4 ) { // WORD BYTE
						if ( j == num ) {
							return false ;
						} // if
						else if ( table[j] == 6 ) { // decNum
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else if ( table[j] == 4 && position[j] == 9 ) { // X'F1' or C'EOF'
							j++ ;
							if ( j == num ) {
								return false ;
							} // if
							else if ( table[j] == 6 || table[j] == 7 ) {
								j++ ;
								if ( j == num ) { // 少' 
									return false ;
								} // if
								else if ( table[j] == 4 && position[j] == 9 ) { // 第二個' 
									return NextIsNothing( j, table, position, num ) ;
								} // else if
								else { // 後面是其他東西 
									return false ;
								} // else
							} // else if
							else {
								return false ;
							} // else
						} // else if
						else {
							return false ;
						} // else
					} // else if
					else if ( position[i] == 7 ) { // EQU 後面接什麼都有可能 我難過 
						if ( table[j] == 6 ) { // 後面接數字(dec) 
							return NextIsNothing( j, table, position, num ) ;
						} // if
						else if ( table[j] == 5 ) { // symbol有可能有四則運算(QQ)
							for ( int k = j+1 ; k < num ; k++ ) {
								if ( table[k] == 4 ) {
									if ( position[k] > 5 && position[k] < 2 ) { // 非 + - * /
										return false ;
									} // if
								} // if
							} // for
							return true ;
						} // else
						else if ( table[j] == 4 && position[j] == 4 ) { // *
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else {
							return false ;
						} // else 
					} // else if
				} // else if
				else if ( table[i] == 4 && position[i] == 10 ) { // 註解 
					if ( i == 0 ) {
						return true ;
					}  // if
					else {
						return false ;
					} // else
				} // else if
				i++ ;
			} // while
			
			return false ;
		} // CheckTypeSeq
		
		bool CheckError( string code, string format, int table[], int position[] ) {
			string temp = "" ;
			int i = 0 ;
			int num = 0 ; // 算有幾個token 
			while ( i < format.length() && num < 10 ) {
				if ( format[i] != ')' ) {
					temp = temp + format[i] ;
				} // if
				else {
					temp = temp + format[i] ;
					FindPosition( temp, table[num], position[num] ) ;// 要怎麼存東西ㄚㄚㄚㄚㄚㄚ 要看順序唷 
					num++ ;
					temp = "" ;
				} // else
				i++ ;
				
			} // while
			table[num] = -1 ;
			position[num] = -1 ;
			return !CheckTypeSeq( table, position, num ) ; // 檢查table順序對嗎 
		} // CheckError 
		
		string BigEnglish( string temp ) {
			for ( int i = 0 ; i < temp.length() ; i++ ) {
				if ( temp[i] <= 'z' && temp[i] >= 'a' ) {
					temp[i] = temp[i] - 32 ; // 小寫轉大寫 
				} // if
			} // for
			
			return temp ;
		} // BigEnglish
		
		void SeparateWord( string code, int table[], int position[], Output &o ) {
			o.allInstr = code ; // 整個指令! 
			int i = 0 ;
			int num = position[0] ;
			if ( table[i] == 5 ) { // 定義symbol! 
				o.instruction.symbol.name = BigEnglish( table5[num] ) ; // 第一個如果是symbol就要定義symbol，但是如果不是開頭就是指令 
				o.instruction.symbol.table = 5 ;
				o.instruction.symbol.position = num ;
				i++ ;
			} // if
			num = position[i] ;
			int t1to4Num = num - 1 ; // 因為table1~4都是從1開始放的 所以讀到的數字要-1 
			int opnum = form[t1to4Num].opnum ; // form是依照table1做的所以也要-1 
			if ( table[i] == 1 ) {
				o.instruction.instr.name = BigEnglish( table1.at(t1to4Num) ) ;
				o.instruction.instr.table = 1 ;
				o.instruction.instr.position = t1to4Num ;
				if ( opnum != 0 ) { // 有OP 
					i++ ;
					num = position[i] ;
					t1to4Num = num - 1 ;
					if ( table[i] == 3 ) { // op1是reg 
						o.instruction.op1.name = table3.at(t1to4Num) ;
						o.instruction.op1.table = 3 ;
						o.instruction.op1.position = t1to4Num ;
					} // if
					else if ( table[i] == 5 ) { // op1是symbol 
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // else if
					else if ( table[i] == 6 ) { // 直接是數字 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					}  // else if
					else if ( ( table[i] == 4 && ( 11 <= num ) && ( num <= 13 ) ) ) { // = # @的情況 
						i++ ;
						num = position[i] ;
						if ( num == 12 ) { // #
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "#" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
							else if ( table[i] == 6 ) {
								o.instruction.op1.name = "#" + BigEnglish( table6[num] ) ;
								o.instruction.op1.table = 6 ;
								o.instruction.op1.position = num ;
							} // else if	
						} // if
						else if ( num == 13 ) { // @ 只接symbol 
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "@" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
						} // else if
						else { // =
							if ( table[i] == 4 && position[i] == 9 ) { // C'' X''
								i++ ;
								num = position[i] ;
								if ( table[i] == 6 ) { // X''
									o.instruction.op1.name = "X'" + BigEnglish( table6[num] ) + "'" ;
									o.instruction.op1.table = 6 ;
									o.instruction.op1.position = num ;
								} // if
								else { // C''
									o.instruction.op1.name = "C'" + table7[num] + "'" ; // 不能轉大寫 我還要算ASCII 
									o.instruction.op1.table = 7 ;
									o.instruction.op1.position = num ;
								} // else
							} // if
						} // else
					} // else if
					// op1放好嚕!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					i++ ;
					if ( opnum == 1 ) {
						if ( table[i] == 4 && position[i] == 1 ) { // symbol後有,-->已經確認文法沒錯代表是,X的情況 
							o.instruction.X = true ; // 有X!!!!!! 
							o.instruction.op2.name = "X" ;
							o.instruction.op2.table = 3 ;
							o.instruction.op2.position = 2 ; // X reg
						} // if
					}  // if
					if ( opnum == 2 ) { // 有op2!!! 
						if ( table[i] == 4 && position[i] == 1 ) { // symbol後有,
							i++ ;
							if ( table[i] == 3 ) { // op2是reg 
								o.instruction.op2.name = table3.at(position[i]-1) ; // 當初存位置有+1 t1~4 
								o.instruction.op2.table = 3 ;
								o.instruction.op2.position = position[i]-1 ;
							} // if
							else { // op2是數字 
								o.instruction.op2.name = table6[position[i]] ;
								o.instruction.op2.table = 6 ;
								o.instruction.op2.position = position[i] ;
							} // else
						} // if
					}  // if
				} // if
			} // if
			else if ( table[i] == 2 ) { // 定義symbol 後面一定是table1 or 2 除非是註解 
				o.instruction.instr.name = table2.at(t1to4Num) ;
				o.instruction.instr.table = 2 ;
				o.instruction.instr.position = t1to4Num ;
				i++ ;
				if ( num == 3 || num == 4 ) { // WORD BYTE
					num = position[i] ;
					if ( table[i] == 6 ) { //後面直接是數字 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && num == 9 ) { // C'EOF' X'F1'
						i++ ;
						num = position[i] ;
						t1to4Num = num - 1 ;
						if ( table[i] == 6 ) { // X'F1'
							o.instruction.op1.name = "X'" + BigEnglish( table6[num] ) + "'" ;
							o.instruction.op1.table = 6 ;
							o.instruction.op1.position = num ;
						} // if
						else if ( table[i] == 7 ) { // C'EOF'
							o.instruction.op1.name = "C'" + table7[num] + "'" ;
							o.instruction.op1.table = 7 ;
							o.instruction.op1.position = num ;
						} // else if
					} // else if
				} // if
				else if ( num == 1 ) { // START後面一定接數字 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 8 ) { // BASE後面接symbol或decnum 
					num = position[i] ;
					if ( table[i] == 5 ) {
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // if
					else {
						o.instruction.op1.name = BigEnglish( table6[num] ) ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // else
				} // else if
				else if ( num == 5 || num == 6 ) { // RESW RESB 後面只接數字 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 7 ) { // EQU 後面可能是一堆奇怪的東西@@ 直接全部存! 
					string temp = "" ;
					if ( table[i] == 6 ) {
						num = position[i] ;
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && position[i] == 4 ) { // *
						o.instruction.op1.name = "*" ;
						o.instruction.op1.table = 4 ;
						o.instruction.op1.position = 4 ;
					} // else if
					else { // label 可能有四則運算 
						for ( int j = 0 ; j < code.length() ; j++ ) {
							if ( code[j] != '	' ) {
								temp = temp + code[j] ;
							} // if
							else {
								if ( temp == "EQU" || temp == "equ" ) { // 無literal 
									temp = "" ;
									temp.append( code, j+1, code.length()-j ) ; // 從j+1開始取出len-j個字放到temp-->剩下的字 
								} // if
								else {
									temp = "" ;
								} // else
							} // else
						} // for
					
						o.instruction.op1.name = BigEnglish( temp ) ;
					} // else
					
				} // else if
				// LTORG後面沒東西@@ END 不做事 
			} // else if
			else if ( table[i] == 4 && num == 10 ) { // 註解! 
				;
			} // else if
		} // SeparateWord()
		
		int LocStrtoNum( string strLoc ) {
			stringstream ss ;
			int loc = 0 ;
			ss << hex << strLoc ; // 將字串以16進制讀入 
			ss >> loc ; // 輸出成10進位 (100=>256) 
			ss.clear() ; // 要記得清空!
			return loc ;
		} // LocStrtoNum
		
		string LocNumtoStr( int loc ) {
			stringstream ss ;
			string strLoc = "" ;
			ss << hex << loc ; // 將loc轉成16進位
			ss >> strLoc ; // 輸出成字串 
			ss.clear() ;
			strLoc = BigEnglish( strLoc ) ;
			return strLoc ;
		} // LocStrtoNum
		
		string FillStr( string temp, int num ) {
			while ( temp.length() < num ) {
				temp = "0" + temp ;
			} // while
			
			return temp ;
		} // FillStr
		
		void TranslatePass1() { // Pass1囉! literal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			string strLoc = "0000" ; // 字串的location 
			int loc = 0 ; // 計算用的loc
			int i = 0 ;
			stringstream ss ; // 轉換進制 
			int move = 0 ; // BYTE WORD用 
			string temp = "" ; // C''||EQU用 
			int num = 0 ; // C''用 
			while ( i < output.size() ) {
				Symbol s ;
				Output o = output.at(i) ;
				SIC ins = o.instruction ;
				if ( ins.instr.name == "START" ) { // 設定程式起始位置! 不用翻機器碼 
					strLoc = ins.op1.name ; // 程式起始位置
					strLoc = BigEnglish( strLoc ) ; // 原本LocNumtoStr已經有轉大寫 可是這裡是一開始 沒用到 所以要轉 
					loc = LocStrtoNum( strLoc ) ;  
					strLoc = FillStr( strLoc, 4 ) ; // 填滿滿4個 
					output.at(i).loc = strLoc ;
					if ( ins.symbol.name != "" ) {
						s.name = ins.symbol.name ; // 將symbol存入symboltable 
						s.address = strLoc ;
						for ( int k = 0 ; k < symtab.size() ; k++ ) {
							if ( symtab.at(k).name == ins.symbol.name ) {
								output.at(i).error = true ;
								output.at(i).code = "Duplicated Definition!" ;
							} // if
						} // for
						if ( !output.at(i).error ) { // 沒問題的指令 就可以push 
							symtab.push_back( s ) ;
						} // if
					} // if
				} // if
				else {
					if ( ins.instr.name != "" ) { // 不是註解或者空的 
						if ( ins.instr.name != "END" && ins.instr.name != "BASE" ) { // 沒結束  因為BASE也不用算LOC 
							if ( !output.at(i).error ) { // 沒錯的指令才要處理 
								if ( ins.instr.name == "RESB" ) {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // 字串轉數字 
									ss >> move ;
									ss.clear() ;
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
								} // if
								else if ( ins.instr.name == "RESW" ) {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // 字串轉數字 
									ss >> move ;
									ss.clear() ;
									move = move * 3 ; // word有3byte
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
								} // else if
								else if ( ins.instr.name == "BYTE" || ins.instr.name == "WORD" ) {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									if ( ins.op1.name[0] == 'C' ) { // C'EOF' || X'F1' 
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												num = ins.op1.name[j] ;
												output.at(i).code = output.at(i).code + LocNumtoStr( num ) ;
											} // if
											else {
												break ;
											} // else
										} // for
										move = output.at(i).code.length() ;
										output.at(i).code = BigEnglish( output.at(i).code ) ;
										if ( ins.instr.name == "WORD" ) { // 要填滿6個指令 
											output.at(i).code = FillStr( output.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code長度為奇數 
											output.at(i).code = "0" + output.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // if
									else if ( ins.op1.name[0] == 'X' ) {
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												output.at(i).code = output.at(i).code + ins.op1.name[j] ;
											} // if
											else {
												break ;
											} // else
										} // for
										output.at(i).code = BigEnglish( output.at(i).code ) ;
										move = output.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // 要填滿6個指令 
											output.at(i).code = FillStr( output.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code長度為奇數 
											output.at(i).code = "0" + output.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else if
									else { // dec num
										output.at(i).loc = FillStr( strLoc, 4 ) ;
										ss << dec << ins.op1.name ; // 字串轉數字 
										ss >> num ; 
										ss.clear() ;
										ss << hex << num ; // 將數字以16進制讀入轉成字串 
										ss >> output.at(i).code ;
										ss.clear() ;
										output.at(i).code = BigEnglish( output.at(i).code ) ;
										move = output.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // 要填滿6個指令 
											output.at(i).code = FillStr( output.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code長度為奇數 
											output.at(i).code = "0" + output.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else
								} // else if
								else if ( ins.instr.name == "EQU" ) { // 不影響下一道指令的位置 
									if ( ins.op1.table == 6 ) {
										ss << dec << ins.op1.name ;
										ss >> move ;
										ss.clear() ;
										temp = "" ;
										ss << hex << move ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										output.at(i).loc = FillStr( temp, 4 ) ;
									} // if
									else if ( ins.op1.table == 4 ) { // * 直接是正常算的address 
										output.at(i).loc = FillStr( strLoc, 4 ) ;
										// 下一道指令LOC一樣 
									} // else if
									else { // label囉! 
										temp = ins.op1.name ;
										string t = "" ; // 暫存symbol 
										int sum = 0 ; // 數最後Loc
										char c = '\0' ; 
										for ( int j = 0 ; j < temp.length() ; j++ ) {
											if ( temp[j] != '+' || temp[j] != '*' || temp[j] != '-' || temp[j] != '/' ) {
												t = t + temp[j] ;
											} // if
											else {
												ss << hex << FindSymLoc( t ) ;
												ss >> move ;
												ss.clear() ;
												if ( sum == 0 ) {
													sum = move ;
												} // if
												else if ( c == '-' ) {
													sum = sum - move ;
												} // else if
												else if ( c == '+' ) {
													sum = sum + move ;
												} // else if
												else if ( c == '*' ) {
													sum = sum * move ;
												} // else if
												else if ( c == '/' ) {
													sum = sum / move ;
												} // else if
												c = temp[j] ;
											} // else
										} // for
										if ( sum == 0 ) { // 有可能就一個label 
											ss << hex << FindSymLoc( t ) ;
											ss >> move ;
											ss.clear() ;
											sum = move ;
										} // if
										else if ( c == '-' ) { // 最後一個symbol沒計算到 
											sum = sum - move ;
										} // else if
										else if ( c == '+' ) {
											sum = sum + move ;
										} // else if
										else if ( c == '*' ) {
											sum = sum * move ;
										} // else if
										else if ( c == '/' ) {
											sum = sum / move ;
										} // else if
										ss << hex << sum ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										output.at(i).loc = FillStr( temp, 4 ) ;
									} // else
								} // else if
								else {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									loc = loc + 3 ; // 固定+3 
									strLoc = LocNumtoStr( loc ) ;
								} // else
							} // if
						} // if
						if ( ins.symbol.name != "" ) { // 有要定義symbol 
							s.name = ins.symbol.name ; // 將symbol存入symboltable 
							s.address = output.at(i).loc ;
							for ( int k = 0 ; k < symtab.size() ; k++ ) {
								if ( symtab.at(k).name == ins.symbol.name ) {
									output.at(i).error = true ;
									output.at(i).code = "Duplicated Definition!" ;
								} // if
							} // for
							if ( !output.at(i).error ) { // 沒問題的指令 就可以push 
								symtab.push_back( s ) ;
							} // if
						} // if
						// END不做事 
					} // if
					// 註解或空的也不做事! 
				} // else
				i++ ;
			} // while
		} // TranslatePass1
		
		void TranslatePass2() {
			stringstream ss ;
			string addr = "" ; // 翻機器碼需要用binary去轉換 
			string temp = "" ;
			int x = 0 ;
			for ( int i = 0 ; i < output.size() ; i++ ) {
				Output o = output.at(i) ;
				SIC instr = o.instruction ;
				if ( o.code == "" && instr.instr.name != "" ) { // 文法正確&還沒算的&非註解 
					if ( instr.instr.table == 1 ) { // 要查optable要是table1 
						if ( instr.op1.name == "" ) { // 沒有operand 
							output.at(i).code = form[instr.instr.position].opcode + "0000" ; 
							output.at(i).allInstr = output.at(i).allInstr + "\t" ;
						} // if
						else if ( instr.op1.table == 5 ) { // 找symtab 
							addr = FindSymLoc( instr.op1.name ) ;
							if ( addr != "" ) { // 找不到loc-->沒定義的symbol 
								if ( instr.X ) {
									temp.append( addr, 0, 1 ) ; // 最高位元拿出來 要+8(X=1) 
									ss << hex << temp ;
									ss >> x ;
									ss.clear() ;
									x = x + 8 ;
									ss << hex << x ;
									ss >> temp ;
									addr[0] = temp[0] ;
									ss.clear() ;
								} // if
								output.at(i).code = form[instr.instr.position].opcode + addr ;
							} // if
							else { // 處理Undefine Symbol-->但LOC會有問題(測資沒有吧) 
								for ( int j = 0 ; j < symtab.size() ; j++ ) {
									if ( symtab.at(j).name == o.instruction.symbol.name ) {
										symtab.erase( symtab.begin() + j ) ;
										break ;
									} // if
								} // for
								output.at(i).loc = "" ;
								output.at(i).code = "Undefine Symbol!" ;
								output.at(i).error = true ;
							} // else
						} // else if
					} // if
					
				} // if
			} // for
		} // TranslatePass2
		
		string FindSymLoc( string temp ) {
			for ( int i = 0 ; i < symtab.size() ; i++ ) {
				if ( symtab.at(i).name == temp ) {
					return symtab.at(i).address ;
				} // if
			} // for
			string t = "" ;
			return t ;
		} // FindSymtab
		
		void OutputSIC() {
			string name = fileName + "_SICMachineCode.txt" ;
			FILE *file = NULL ; // 檔案指標 
			file = fopen( name.c_str(), "w" ) ; 
	  		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
			if ( file == NULL ) {
				cout << endl << "檔案開啟失敗!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				int k = 0 ;
				fprintf( file, "Line	Loc	Source statement		Object code\n" ) ;
				for ( int j = 0 ; j < output.size() ; j++ ) {
					Output o = output.at(j) ;
					if ( o.allInstr != "" ) {
						k = k + 5 ;	
						fprintf( file, "%d	%s	%s	", k, o.loc.c_str(), o.allInstr.c_str() ) ;
						//cout << k << "\t" << o.loc << "\t" << o.allInstr << "\t" ; 
						if ( !o.instruction.X ) {
							fprintf( file, "\t" ) ;
							//cout << "\t" ;
						} // if
						fprintf( file, "%s\n", o.code.c_str() ) ;
						//cout << o.code << endl ;
					} // if
					else {
						fprintf( file, "\n" ) ;
					} // else
					
				} // for
			} // else 	
	
			fclose( file ) ;
		} // OutputSIC
		
		void TurnMachineCodeSIC() { // 轉機器碼 SIC
			int line = 0 ;
			int table[10] = {0} ;
			int position[10] = {0} ;
			while ( line < input.size() ) {
				string code = input.at(line) ; // 拿出第line行的東西
				line++ ;
				Output o ;
				while ( ( code == "" || code == "\n" ) && line < input.size()-1 ) {
					if ( line % 2 == 0 ) { // index從0開始 
						output.push_back( o ) ; // 空的也要印!
					} // if
					code = input.at(line) ; // 拿出第line行的東西
					line++ ;
				} // while
				string format = input.at(line) ; // 拿出第line行的東西
				if ( format != "" ) { // 不是空的才判斷 
					if ( !CheckError( code, format, table, position ) ) { // 檢查文法有沒有錯-->無error 
						SeparateWord( code, table, position, o ) ; // 分割好字了! 
					} // if
					else { // 輸出error!!!!!!!!!!! 
						o.error = true ;
						o.allInstr = code ;
						o.code = "Syntax Error!" ;
					} // else
				} // if 
				output.push_back( o ) ; // 空的也要印!
				line++ ;
			} // while
			
			TranslatePass1() ; // 做pass1的事 算loc 給symbol值 
			TranslatePass2() ; 
			OutputSIC() ;
		} // TurnMachineCodeSIC
		
		
		void DoHash( string temp, string type ) {
			int i = 0 ;
			int sum = 0 ;
			string allName = "=" + type + "'" + temp + "'" ;
			bool done = false ;
			while ( i < temp.length() ) {	
				sum = sum + temp[i] ;
				i++ ;
			} // while
			
			sum = sum % 100 ;
			while ( literal[sum] != "" ) {
				if ( literal[sum] == allName ) {
					done = true ;
					break ;
				} // if
				else {
					sum++ ;
					if ( sum == 100 ) {
						sum = 0 ;
					} // if
				} // else
				
			} // while
			if ( !done ) {
				literal[sum] = allName ;
			} // if
			
		} // DoHash
		
		void SeparateWordXE( string code, int table[], int position[], OutputXE &o ) {
			o.allInstr = code ; // 整個指令! 
			int i = 0 ;
			int num = position[0] ;
			if ( table[i] == 5 ) { // 定義symbol! 
				o.instruction.symbol.name = BigEnglish( table5[num] ) ; // 第一個如果是symbol就要定義symbol，但是如果不是開頭就是指令 
				o.instruction.symbol.table = 5 ;
				o.instruction.symbol.position = num ;
				i++ ;
			} // if
			if ( table[i] == 4 && position[i] == 2 ) { // + type4!
				o.type4 = true ;
				i++ ;
			} // if
			num = position[i] ;
			int t1to4Num = num - 1 ; // 因為table1~4都是從1開始放的 所以讀到的數字要-1 
			int opnum = form[t1to4Num].opnum ; // form是依照table1做的所以也要-1 
			if ( table[i] == 1 ) {
				o.instruction.instr.name = BigEnglish( table1.at(t1to4Num) ) ;
				o.instruction.instr.table = 1 ;
				o.instruction.instr.position = t1to4Num ;
				if ( opnum != 0 ) { // 有OP 
					i++ ;
					num = position[i] ;
					t1to4Num = num - 1 ;
					if ( table[i] == 3 ) { // op1是reg 
						o.instruction.op1.name = table3.at(t1to4Num) ;
						o.instruction.op1.table = 3 ;
						o.instruction.op1.position = t1to4Num ;
					} // if
					else if ( table[i] == 5 ) { // op1是symbol 
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // else if
					else if ( table[i] == 6 ) { // 直接是數字 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					}  // else if
					else if ( ( table[i] == 4 && ( 11 <= num ) && ( num <= 13 ) ) ) { // = # @的情況 
						i++ ;
						if ( num == 12 ) { // #
							num = position[i] ;
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "#" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
							else if ( table[i] == 6 ) {
								o.instruction.op1.name = "#" + BigEnglish( table6[num] ) ;
								o.instruction.op1.table = 6 ;
								o.instruction.op1.position = num ;
							} // else if	
						} // if
						else if ( num == 13 ) { // @ 只接symbol 
							num = position[i] ;
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "@" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
						} // else if
						else { // =
							if ( table[i] == 4 && position[i] == 9 ) { // C'' X''
								i++ ;
								num = position[i] ;
								int d = 0 ;
								if ( table[i] == 6 ) { // X''
									for ( d = 0 ; d < o.allInstr.length() ; d++ ) {
										if ( o.allInstr[d] != ' ' && o.allInstr[d] != '	' && o.allInstr[d] != '+' ) {
											break ;
										} // if
									} // for
									if ( o.allInstr[d] >= 'a' && o.allInstr[d] <= 'z' ) {
										
										for ( int k = 0 ; k < 100 ; k++ ) {
											int m = 0 ;
											while ( table6[k][m] >= 'A' && table6[k][m] <= 'Z' ) {
												table6[k][m] = table6[k][m] + 32 ;
											} // if
										} // for
										o.instruction.op1.name = "=x'" + table6[num] + "'" ;
									} // if
									else {
										o.instruction.op1.name = "=X'" + table6[num] + "'" ;
									} // else
									o.instruction.op1.table = 6 ;
									o.instruction.op1.position = num ;
								} // if
								else { // C''
									for ( d = 0 ; d < o.allInstr.length() ; d++ ) {
										if ( o.allInstr[d] != ' ' && o.allInstr[d] != '	' && o.allInstr[d] != '+' ) {
											break ;
										} // if
									} // for
									if ( o.allInstr[d] >= 'a' && o.allInstr[d] <= 'z' ) {
										for ( int k = 0 ; k < 100 ; k++ ) {
											int m = 0 ;
											while ( table6[k][m] >= 'A' && table6[k][m] <= 'Z' ) {
												table6[k][m] = table6[k][m] + 32 ;
											} // if
										} // for
										o.instruction.op1.name = "=c'" + table7[num] + "'" ;
									} // if
									else {
										o.instruction.op1.name = "=C'" + table7[num] + "'" ; 
									} // else
								 	// 不能轉大寫 我還要算ASCII 
									o.instruction.op1.table = 7 ;
									o.instruction.op1.position = num ;
								} // else
							} // if
						} // else
					} // else if
					// op1放好嚕!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					i++ ;
					if ( opnum == 1 ) {
						if ( table[i] == 4 && position[i] == 1 ) { // symbol後有,-->已經確認文法沒錯代表是,X的情況 
							o.instruction.X = true ; // 有X!!!!!! 
							o.instruction.op2.name = "X" ;
							o.instruction.op2.table = 3 ;
							o.instruction.op2.position = 2 ; // X reg
						} // if
					}  // if
					if ( opnum == 2 ) { // 有op2!!! 
						if ( table[i] == 4 && position[i] == 1 ) { // symbol後有,
							i++ ;
							if ( table[i] == 3 ) { // op2是reg 
								o.instruction.op2.name = table3.at(position[i]-1) ; // 當初存位置有+1 t1~4 
								o.instruction.op2.table = 3 ;
								o.instruction.op2.position = position[i]-1 ;
							} // if
							else { // op2是數字 
								o.instruction.op2.name = table6[position[i]] ;
								o.instruction.op2.table = 6 ;
								o.instruction.op2.position = position[i] ;
							} // else
						} // if
					}  // if
				} // if
			} // if
			else if ( table[i] == 2 ) { // 定義symbol 後面一定是table1 or 2 除非是註解 
				o.instruction.instr.name = table2.at(t1to4Num) ;
				o.instruction.instr.table = 2 ;
				o.instruction.instr.position = t1to4Num ;
				i++ ;
				if ( num == 3 || num == 4 ) { // WORD BYTE
					num = position[i] ;
					if ( table[i] == 6 ) { //後面直接是數字 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && num == 9 ) { // C'EOF' X'F1'
						i++ ;
						num = position[i] ;
						t1to4Num = num - 1 ;
						if ( table[i] == 6 ) { // X'F1'
							o.instruction.op1.name = "X'" + BigEnglish( table6[num] ) + "'" ;
							o.instruction.op1.table = 6 ;
							o.instruction.op1.position = num ;
						} // if
						else if ( table[i] == 7 ) { // C'EOF'
							o.instruction.op1.name = "C'" + table7[num] + "'" ;
							o.instruction.op1.table = 7 ;
							o.instruction.op1.position = num ;
						} // else if
					} // else if
				} // if
				else if ( num == 1 ) { // START後面一定接數字 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 8 ) { // BASE後面接symbol或decnum 
					num = position[i] ;
					if ( table[i] == 5 ) {
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // if
					else {
						o.instruction.op1.name = BigEnglish( table6[num] ) ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // else
				} // else if
				else if ( num == 5 || num == 6 ) { // RESW RESB 後面只接數字 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 7 ) { // EQU 後面可能是一堆奇怪的東西@@ 直接全部存! 
					string temp = "" ;
					if ( table[i] == 6 ) {
						num = position[i] ;
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && position[i] == 4 ) { // *
						o.instruction.op1.name = "*" ;
						o.instruction.op1.table = 4 ;
						o.instruction.op1.position = 4 ;
					} // else if
					else { // label 可能有四則運算 
						for ( int j = i ; table[j] != -1 ; j++ ) {
							num = position[j] ;
							if ( table[j] == 5 ) {
								temp = temp + table5[num] ;
							} // if
							else if ( table[j] == 4 ) {
								temp = temp + table4.at(num-1) ;
							} // else if
						} // for
						o.instruction.op1.name = BigEnglish( temp ) ;
						o.instruction.op1.table = 5 ;
					} // else
					
				} // else if
				// LTORG後面沒東西@@ END 不做事 
			} // else if
			else if ( table[i] == 4 && num == 10 ) { // 註解! 
				;
			} // else if
		} // SeparateWordXE()
		
		bool All16Num( string temp ) { // 用在分辨address和symbol SIC無address 
			for ( int j = 0 ; j < temp.length() ; j++ ) { // 確認是address還是symbol 
				if ( temp[j] >= '0' && temp[j] <= '9' ) { // 如果是數字的話就沒事 
					;
				} // if
				else if ( temp[j] >= 'A' && temp[j] <= 'F' ) { // opname都是大寫的 
					;
				} // else if
				else {
					return false ;
				} // else
			} // for
			return true ;
		} // All16Num
		
		void TranslatePass1XE() { // SICXE Pass1囉! literal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			string strLoc = "0000" ; // 字串的location 
			int loc = 0 ; // 計算用的loc
			int i = 0 ;
			stringstream ss ; // 轉換進制 
			int move = 0 ; // BYTE WORD用 
			string temp = "" ; // C''||EQU END LTORG用 
			string code = "" ; // END LTORG用 
			string t1 = "" ;
			int num = 0 ; // C''用 
			while ( i < outXE.size() ) {
				Symbol s ;
				OutputXE o = outXE.at(i) ;
				SIC ins = o.instruction ;
				if ( ins.instr.name == "START" ) { // 設定程式起始位置! 不用翻機器碼 
					strLoc = ins.op1.name ; // 程式起始位置
					loc = LocStrtoNum( strLoc ) ; 
					baseReg = loc ; // 存baseReg位置 
					strLoc = BigEnglish( strLoc ) ; // 英文轉大寫阿
					strLoc = FillStr( strLoc, 4 ) ; // 填滿滿4個 
					outXE.at(i).loc = strLoc ;
					outXE.at(i).PC = loc ; // 要存PC! 
					if ( ins.symbol.name != "" ) {
						s.name = ins.symbol.name ; // 將symbol存入symboltable 
						s.address = strLoc ;
						for ( int k = 0 ; k < symtab.size() ; k++ ) {
							if ( symtab.at(k).name == ins.symbol.name ) {
								outXE.at(i).error = true ;
								outXE.at(i).code = "Duplicated Definition!" ;
								break ;
							} // if
						} // for
						if ( !outXE.at(i).error ) { // 沒問題的指令 就可以push 
							symtab.push_back( s ) ;
						} // if
					} // if
				} // if
				else {
					if ( ins.instr.name != "" ) { // 不是註解或者空的 
						if ( ins.instr.name != "END" && ins.instr.name != "BASE" && ins.instr.name != "LTORG" ) { // 沒結束  因為BASE也不用算LOC 
							if ( !outXE.at(i).error ) { // 沒錯的指令才要處理 
								if ( ins.instr.name == "RESB" ) {
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // 字串轉數字 
									ss >> move ;
									ss.clear() ;
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
								} // if
								else if ( ins.instr.name == "RESW" ) {
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // 字串轉數字 
									ss >> move ;
									ss.clear() ;
									move = move * 3 ; // word有3byte
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
									outXE.at(i).PC = loc ; // 要存PC!
								} // else if
								else if ( ins.instr.name == "BYTE" || ins.instr.name == "WORD" ) {
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									if ( ins.op1.name[0] == 'C' ) { // C'EOF' || X'F1' 
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												num = ins.op1.name[j] ;
												outXE.at(i).code = outXE.at(i).code + LocNumtoStr( num ) ;
											} // if
											else {
												break ;
											} // else
										} // for
										move = outXE.at(i).code.length() ;
										outXE.at(i).code = BigEnglish( outXE.at(i).code ) ;
										if ( ins.instr.name == "WORD" ) { // 要填滿6個指令 
											outXE.at(i).code = FillStr( outXE.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code長度為奇數 
											outXE.at(i).code = "0" + outXE.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // if
									else if ( ins.op1.name[0] == 'X' ) {
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												outXE.at(i).code = outXE.at(i).code + ins.op1.name[j] ;
											} // if
											else {
												break ;
											} // else
										} // for
										outXE.at(i).code = BigEnglish( outXE.at(i).code ) ;
										move = outXE.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // 要填滿6個指令 
											outXE.at(i).code = FillStr( outXE.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code長度為奇數 
											outXE.at(i).code = "0" + outXE.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else if
									else { // dec num
										outXE.at(i).loc = FillStr( strLoc, 4 ) ;
										ss << dec << ins.op1.name ; // 字串轉數字 
										ss >> num ; 
										ss.clear() ;
										ss << hex << num ; // 將數字以16進制讀入轉成字串 
										ss >> outXE.at(i).code ;
										ss.clear() ;
										outXE.at(i).code = BigEnglish( outXE.at(i).code ) ;
										move = outXE.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // 要填滿6個指令 
											outXE.at(i).code = FillStr( outXE.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code長度為奇數 
											outXE.at(i).code = "0" + outXE.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else
									outXE.at(i).PC = loc ; // 要存PC!
								} // else if
								else if ( ins.instr.name == "EQU" ) { // 不影響下一道指令的位置 
									if ( ins.op1.table == 6 ) {
										ss << dec << ins.op1.name ;
										ss >> move ;
										ss.clear() ;
										temp = "" ;
										ss << hex << move ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										outXE.at(i).loc = FillStr( temp, 4 ) ;
										if ( i > 0 ) { // 不是第一個 有前一個人的PC需要改 
											outXE.at(i-1).PC = move ; // 要存PC!
										} // if
										outXE.at(i).PC = loc ; // 要存PC!
									} // if
									else if ( ins.op1.table == 4 ) { // * 直接是正常算的address 
										outXE.at(i).loc = FillStr( strLoc, 4 ) ;
										outXE.at(i).PC = loc ; // 要存PC!
										// 下一道指令LOC一樣 
									} // else if
									else { // label囉! 
										temp = ins.op1.name ;
										string t = "" ; // 暫存symbol 
										int sum = 0 ; // 數最後Loc
										char c = '\0' ; 
										for ( int j = 0 ; j < temp.length() ; j++ ) {
											if ( temp[j] != '+' && temp[j] != '*' && temp[j] != '-' && temp[j] != '/' ) {
												t = t + temp[j] ;
											} // if
											else {
												ss << hex << FindSymLoc( t ) ;
												ss >> move ;
												ss.clear() ;
												t = "" ;
												if ( sum == 0 ) {
													sum = move ;
												} // if
												else if ( c == '-' ) {
													sum = sum - move ;
												} // else if
												else if ( c == '+' ) {
													sum = sum + move ;
												} // else if
												else if ( c == '*' ) {
													sum = sum * move ;
												} // else if
												else if ( c == '/' ) {
													sum = sum / move ;
												} // else if
												c = temp[j] ;
											} // else
										} // for
										ss << hex << FindSymLoc( t ) ;
										ss >> move ;
										ss.clear() ;
										if ( sum == 0 ) { // 有可能就一個label 
											sum = move ;
										} // if
										else if ( c == '-' ) { // 最後一個symbol沒計算到 
											sum = sum - move ;
										} // else if
										else if ( c == '+' ) {
											sum = sum + move ;
										} // else if
										else if ( c == '*' ) {
											sum = sum * move ;
										} // else if
										else if ( c == '/' ) {
											sum = sum / move ;
										} // else if
										ss << hex << sum ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										outXE.at(i).loc = FillStr( temp, 4 ) ;
										if ( i > 0 ) { // 如果有前一個人 他的PC要改 
											outXE.at(i-1).PC = sum ; // 要存PC!
										} // if
										outXE.at(i).PC = loc ; // 要存PC!
									} // else
								} // else if
								else if ( ins.instr.table == 1 ) { // 指令!
									num = ins.instr.position ;
									int type = form[num].type ;
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									if ( type == 1 ) { // 只有opcode 1byte 
										loc = loc + 1 ; // 1byte 
										strLoc = LocNumtoStr( loc ) ;
									}  // if
									else if ( type == 2 ) { // 有opcode r1 r2 --> 共2byte 
										loc = loc + 2 ; // 4byte 
										strLoc = LocNumtoStr( loc ) ;
									} // else if
									else { // 只剩下type3 or 4 --> 3:3byte, 4:4byte 
										if ( o.type4 ) {
											loc = loc + 4 ; // 8byte 
											strLoc = LocNumtoStr( loc ) ;
										} // if
										else {
											loc = loc + 3 ; // 6byte 
											strLoc = LocNumtoStr( loc ) ;
										} // else
									} // else
									outXE.at(i).PC = loc ; // 要存PC!
								} // else
							} // if
						} // if
						else if ( ins.instr.name == "LTORG" || ins.instr.name == "END" ) { // 不用存PC
							// literal 不能一次放 要看到LTORG後才放之前看到的 未解決 
							for ( int j = 0 ; j < 100 ; j++ ) {
								if ( literal[j] != "" ) { // 要成為指令囉! 
									OutputXE lit ;
									lit.allInstr = literal[j] + "	BYTE	" ;
									temp = literal[j].substr( 3, literal[j].length()-4 ) ; // EOF || F1 
									if ( literal[j][1] == 'C' || literal[j][1] == 'c' ) {
										for ( int n = 0 ; n < temp.length() ; n++ ) {
											num = temp[n] ;
											lit.code = lit.code + LocNumtoStr( num ) ; ;
										} // for
									} // if
									else if ( literal[j][1] == 'X' || literal[j][1] == 'x' ) {
										lit.code = temp ;
									} // else if
									lit.code = BigEnglish( lit.code ) ;
									move = lit.code.length() ;
									if ( move % 2 != 0 ) { // code長度為奇數 
										lit.code = "0" + lit.code ;
										move++ ;
									} // if
									lit.loc = BigEnglish( strLoc ) ;
									lit.loc = FillStr( lit.loc, 4 ) ;
									lit.instruction.instr.name = "BYTE" ;
									lit.instruction.instr.table = 2 ;
									lit.instruction.instr.position = 3 ;
									lit.instruction.symbol.name = literal[j] ;
									lit.instruction.symbol.table = 5 ;
									Symbol s ;
									s.name = BigEnglish( literal[j] ) ;
									s.address = lit.loc ;
									symtab.push_back( s ) ;
									move = move / 2 ;
									loc = loc + move ; // code長度/2 
									strLoc = LocNumtoStr( loc ) ;
									strLoc = FillStr( strLoc, 4 ) ;
									i++ ; // 會插在前面故+1 
									outXE.insert( ( outXE.begin() + i ), lit ) ;
									outXE.at(i).PC = loc ; // 要存PC!
									literal[j] = "" ; // 清空table 
								}  // if
							} // for
						} // else if
						// BASE 不做事(pass2才改baseReg) 
						if ( ins.symbol.name != "" ) { // 有要定義symbol 
							s.name = ins.symbol.name ; // 將symbol存入symboltable 
							s.address = outXE.at(i).loc ;
							for ( int k = 0 ; k < symtab.size() ; k++ ) {
								if ( symtab.at(k).name == ins.symbol.name ) {
									outXE.at(i).error = true ;
									outXE.at(i).code = "Duplicated Definition!" ;
									break ;
								} // if
							} // for
							if ( !outXE.at(i).error ) { // 沒問題的指令 就可以push 
								symtab.push_back( s ) ;
							} // if
						} // if
					} // if
					// 註解或空的也不做事! 
				} // else
				if ( ins.op1.name[0] == '=' ) { // literal 要放入literal table
					temp = ins.op1.name.substr( 3, ins.op1.name.length() - 4 ) ; // 拿EOF||F1 
					string type = "" ;
					type = ins.op1.name.substr( 1, 1 ) ;
					if ( ins.op1.table == 6 ) { // X'F1'
						DoHash( temp, type ) ;
					}  // if
					else {
						DoHash( temp, type ) ;
					} // else
				} // if
				i++ ;
			} // while
		} // TranslatePass1XE
		
		string XEType2( OutputXE o, SIC instr, string opcode ) { // opcode r1{,r2}
			stringstream ss ;
			string op1name = instr.op1.name ;
			string op2name = instr.op2.name ;
			string code = opcode ;
			string temp = "" ;
			int num = instr.op1.position ; // reg的code為他在table的位置
			if ( instr.op1.table == 3 ) {
				ss << dec << num ;
				ss >> temp ;
				ss.clear() ;
			} // if
			else if ( instr.op1.table == 6 ) {
				temp = instr.op1.name ;
			}  // else if
			code = code + temp ; // 差op2 
			if ( op2name == "" ) { // 只有1個op 	
				code = code + "0" ;
			} // if
			else {
				num = instr.op2.position ;
				ss << dec << num ;
				ss >> temp ;
				ss.clear() ;
				code = code + temp ;
			} // else
			code = BigEnglish( code ) ;
			return code ;
		} // XEType2
		
		void TranslatePass2XE() {
			stringstream ss ;
			string addr = "" ; // 翻機器碼需要用binary去轉換 
			string temp = "" ;
			string opcode = "" ;
			int codeNum = 0 ;
			int disp = 0 ;
			int pc = -1 ;
			int index = 0 ;
			string code = "" ;
			for ( int i = 0 ; i < outXE.size() ; i++ ) {
				OutputXE o = outXE.at(i) ;
				SIC instr = o.instruction ;
				if ( o.code == "" && instr.instr.name != "" ) { // 文法正確&還沒算的&非註解 
					if ( instr.instr.table == 1 ) { // 要查optable要是table1
						index = instr.instr.position ; 
						opcode = form[index].opcode ;
						if ( form[index].type == 1 ) { // type1-->沒有operand
							outXE.at(i).code = opcode ; // 不用補0唷 
							outXE.at(i).allInstr = outXE.at(i).allInstr + "\t" ;
						} // if
						else if ( form[index].type == 2 ) { // type2 
							outXE.at(i).code = XEType2( o, instr, opcode ) ;
						} // else if
						else if ( form[index].type == 3 ) { // type3 4 -->op數為0或1(m) 
							string op1name = o.instruction.op1.name ;
							pc = o.PC ;
							bool imm = false ;
							bool fin = false ;
							code = opcode ;
							codeNum = LocStrtoNum( code ) ; // 將16進位的字串轉為10進位的數字 
							if ( op1name == "" ) { // type3 4 但opnum = 0 
								codeNum = codeNum + 3 ; // 沒有op-->非#非@
								code = LocNumtoStr( codeNum ) ; 
								code = FillStr( code, 2 ) ;
								if ( !o.type4 ) { // type3
									outXE.at(i).code = code + "0000" ; // 後面沒op 所以全部都是0 
								} // if
								else { // type4
									outXE.at(i).code = opcode + "100000" ; // 後面沒op 所以1(xbpe)-->0001
								} // else
							} // if
							else if ( op1name[0] == '#' || op1name[0] == '@' ) { 
								// # opcode + 1 後面可能是symbol或decnum 
								// @ opcode + 2 後面可能是symbol  
								if ( op1name[0] == '#' ) {
									codeNum++ ; // # opcode+1-->opcode算好了
								} // if
								else { // @
									codeNum = codeNum + 2 ;
								} // else
								code = LocNumtoStr( codeNum ) ; // 大寫也轉好了
								code = FillStr( code, 2 ) ;
								temp = op1name.substr( 1, op1name.length()-1 ) ; // 拿#後面的東西 
								if ( instr.op1.table == 5 ) {
									temp = FindSymLoc( temp ) ;
									if ( temp == "" ) { // 找不到loc-->沒定義的symbol 處理Undefine Symbol-->但LOC會有問題(測資沒有吧)	
										for ( int j = 0 ; j < symtab.size() ; j++ ) {
											if ( symtab.at(j).name == o.instruction.symbol.name ) {
												symtab.erase( symtab.begin() + j ) ;
												break ;
											} // if
										} // for
										outXE.at(i).loc = "" ;
										outXE.at(i).code = "Undefine Symbol!" ;
										outXE.at(i).error = true ;
										fin = true ;
									} // if
									else {
										codeNum = LocStrtoNum( temp ) ; // symbol address(int)
									} // else
								} // if
								else { // decnum
									ss << dec << temp ;
									ss >> codeNum ;
									ss.clear() ;
									imm = true ;
								} // else
								if ( !fin && !o.type4 ) { // type3 算距離 
									if ( imm ) { // #3 之類的直接放 
										temp = LocNumtoStr( codeNum ) ;
										code = code + FillStr( temp, 4 ) ; // disp 3bytes + xbpe(1byte)
									} // if
									else { // symbol們 
										disp = codeNum - pc ; // 目的地-PC
										if ( disp >= -2047 && disp <= 2047 ) { // 超出這個範圍要用baseReg 
												addr = LocNumtoStr( disp ) ; // disp 大寫的唷
											addr = FillStr( addr, 3 ) ; // 正的若不足要補0(負的很夠) 
											addr = addr.substr( addr.length()-3, 3 ) ; // 只拿3個(12bit)
											code = code + "2" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
										}  // if
										else { // 用base算 
											disp = codeNum - baseReg ; // 目的-baseReg
											addr = LocNumtoStr( disp ) ; // disp 大寫的唷 
											addr = FillStr( addr, 3 ) ; // 正的若不足要補0(負的很夠) 
											addr = addr.substr( addr.length()-3, 3 ) ; // 只拿3個(12bit)
											code = code + "4" + addr ; // code(opcode)+4(xbpe-->0100)+addr(disp) 
										} // else
									} // else
									outXE.at(i).code = code ;
								} // if
								else if ( !fin ) { // type4 直接放symbol address 
									addr = LocNumtoStr( codeNum ) ; // address(str)
									addr = FillStr( addr, 5 ) ; // loc只有4bytes-->address要5bytes
									code = code + "1" + addr ; // code(opcode) 1(xbpe)0001 addr(addr)
									outXE.at(i).code = code ; 
								} // else if
							} // else if
							else if ( op1name[0] == '=' ) { // literal type3 4
								codeNum = codeNum + 3 ;
								code = LocNumtoStr( codeNum ) ; // opcode翻好了 大寫唷!	
								code = FillStr( code, 2 ) ;
								addr = FindSymLoc( BigEnglish(op1name) ) ; // literal address 
								codeNum = LocStrtoNum( addr ) ; // literal address(int)
								if ( !o.type4 ) { // type3
									disp = codeNum - pc ; // 目的地-PC
									if ( disp >= -2047 && disp <= 2047 ) { // 超出這個範圍要用baseReg 
										addr = LocNumtoStr( disp ) ; // disp 大寫的唷
										addr = FillStr( addr, 3 ) ; // 正的若不足要補0(負的很夠) 
										addr = addr.substr( addr.length()-3, 3 ) ; // 只拿3個(12bit)
										code = code + "2" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
									}  // if
									else { // 用base算 
										disp = codeNum - baseReg ; // 目的-baseReg
										addr = LocNumtoStr( disp ) ; // disp 大寫的唷 
										addr = FillStr( addr, 3 ) ; // 正的若不足要補0(負的很夠) 
										addr = addr.substr( addr.length()-3, 3 ) ; // 只拿3個(12bit)
										code = code + "4" + addr ; // code(opcode)+4(xbpe-->0100)+addr(disp) 
									} // else
									outXE.at(i).code = code ;
								} // if
								else {
									addr = FillStr( addr, 5 ) ; // 要有5個bytes! 
									code = code + "1" + addr ; // code(opcode) 1(xbpe)0001 addr(addr)
									outXE.at(i).code = code ; 
								} // else
							} // else if
							else { // 不是#也不是@也不是literal opcode + 3 
								codeNum = codeNum + 3 ;
								code = LocNumtoStr( codeNum ) ; // opcode翻好了 大寫唷!	
								code = FillStr( code, 2 ) ;	
								if ( All16Num( op1name ) ) { // address
									//codeNum = LocStrtoNum( op1name ) ; // address(int) 
									if ( !o.type4 ) {
										addr = FillStr( op1name, 4 ) ;
										outXE.at(i).code = code + addr ;
										fin = true ;
									} // if
									else {
										addr = FillStr( op1name, 6 ) ;
										outXE.at(i).code = code + addr ;
										fin = true ;
									} // else
								} // if
								else { // symbol
									addr = FindSymLoc( op1name ) ; // symbol address(str)
									if ( addr != "" ) { // 有找到! 
										codeNum = LocStrtoNum( addr ) ; // Symbol address(int)
									} // if
									else { // 處理Undefine Symbol-->但LOC會有問題(測資沒有吧) 
										for ( int j = 0 ; j < symtab.size() ; j++ ) {
											if ( symtab.at(j).name == o.instruction.symbol.name ) {
												symtab.erase( symtab.begin() + j ) ;
												break ;
											} // if
										} // for
										outXE.at(i).loc = "" ;
										outXE.at(i).code = "Undefine Symbol!" ;
										outXE.at(i).error = true ;
										fin = true ;
									} // else
								} // else
								if ( !fin && !o.type4 ) { // 如果沒有未定義symbol type3
									disp = codeNum - pc ;
									if ( disp >= -2047 && disp <= 2047 ) { // 超出這個範圍要用baseReg 
										addr = LocNumtoStr( disp ) ; // disp 大寫的唷
										addr = FillStr( addr, 3 ) ; // 正的若不足要補0(負的很夠) 
										addr = addr.substr( addr.length()-3, 3 ) ; // 只拿3個(12bit)
										if ( instr.X ) {
											code = code + "A" + addr ; // code(opcode)+A(xbpe-->1010)+addr(disp) 
										} // if
										else {
											code = code + "2" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
										} // else
										
									}  // if
									else { // 用base算 
										disp = codeNum - baseReg ; // 目的-baseReg
										addr = LocNumtoStr( disp ) ; // disp 大寫的唷 
										addr = FillStr( addr, 3 ) ; // 正的若不足要補0(負的很夠) 
										addr = addr.substr( addr.length()-3, 3 ) ; // 只拿3個(12bit)
										if ( instr.X ) {
											code = code + "C" + addr ; // code(opcode)+C(xbpe-->1100)+addr(disp) 
										} // if
										else {
											code = code + "4" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
										} // else
									} // else
									outXE.at(i).code = code ;
								} // if
								else if ( !fin ) { // type4
									addr = LocNumtoStr( codeNum ) ; // address(str)
									addr = FillStr( addr, 5 ) ; // loc只有4bytes-->address要5bytes
									if ( instr.X ) {
										code = code + "9" + addr ; // code(opcode) 9(xbpe)1001 addr(addr)
									} // if
									else {
										code = code + "1" + addr ; // code(opcode) 1(xbpe)0001 addr(addr)
									} // else
									outXE.at(i).code = code ; 
								} // else if
							} // else
						} // else if
					} // if
					else if ( instr.instr.table == 2 ) { // 我要更新baseReg 
						if ( instr.instr.name == "BASE" ) { // 算baseReg 
							if ( instr.op1.table == 6 ) { // 10進位數字直接轉 
								ss << dec << instr.op1.name ;
								ss >> dec >> baseReg ;
								ss.clear() ;
							} // if
							else { // symbol
								temp = FindSymLoc( instr.op1.name ) ;
								if ( temp != "" ) {  // 找到了! 
									ss << hex << temp ;
									ss >> baseReg ;
									ss.clear() ;
								} // if
								else { // 沒找到symbol-->LOC有問題唷唷
									for ( int j = 0 ; j < symtab.size() ; j++ ) {
										if ( symtab.at(j).name == o.instruction.symbol.name ) {
											symtab.erase( symtab.begin() + j ) ;
											break ;
										} // if
									} // for
									outXE.at(i).loc = "" ;
									outXE.at(i).code = "Undefine Symbol!" ;
									outXE.at(i).error = true ;
								} // else
							} // else
						} // if
					} // else if
				} // if
			} // for
		} // TranslatePass2XE
		
		void TurnMachineCodeSICXE() { // 轉機器碼 SIC
			int line = 0 ;
			int table[10] = {0} ;
			int position[10] = {0} ;
			while ( line < input.size() ) {
				string code = input.at(line) ; // 拿出第line行的東西
				line++ ;
				OutputXE o ;
				while ( ( code == "" || code == "\n" ) && line < input.size()-1 ) {
					if ( line % 2 == 0 ) { // index從0開始 
						outXE.push_back( o ) ; // 空的也要印!
					} // if
					code = input.at(line) ; // 拿出第line行的東西
					line++ ;
				} // while
				string format = input.at(line) ; // 拿出第line行的東西
				if ( format != "" ) { // 不是空的才判斷 
					if ( !CheckError( code, format, table, position ) ) { // 檢查文法有沒有錯-->無error 
						SeparateWordXE( code, table, position, o ) ; // 分割好字了! 
					} // if
					else { // 輸出error!!!!!!!!!!! 
						o.error = true ;
						o.allInstr = code ;
						o.code = "Syntax Error!" ;
					} // else
				} // if 
				outXE.push_back( o ) ; // 空的也要印!
				line++ ;
			} // while
			TranslatePass1XE() ; // 做pass1的事 算loc 給symbol值 
			TranslatePass2XE() ; 
			OutputSICXE() ;
		} // TurnMachineCodeSICXE
		
		void OutputSICXE() {
			string name = fileName + "_SICXEMachineCode.txt" ;
			FILE *file = NULL ; // 檔案指標 
			file = fopen( name.c_str(), "w" ) ; 
	  		// 打開檔案 r代表讀 c_str()將string物件轉成C語言形式的字串常數(無法任意更改) 
			if ( file == NULL ) {
				cout << endl << "檔案開啟失敗!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				int k = 0 ;
				fprintf( file, "Line  Location  Source code                              Object code\n" ) ;
				for ( int j = 0 ; j < outXE.size() ; j++ ) {
					OutputXE o = outXE.at(j) ;
					if ( o.allInstr != "" ) {
						k = k + 5 ;	
						fprintf( file, "%d	%s	", k, o.loc.c_str() ) ;
						if ( o.instruction.symbol.name == "" && o.allInstr[0] != '\t' ) { // 沒symbol又沒tab 
							fprintf( file, "\t" ) ;
						} // if
						fprintf( file, "%s\t\t", o.allInstr.c_str() ) ;
						if ( !o.instruction.X ) {
							fprintf( file, "\t" ) ;
						} // if
						fprintf( file, "%s\n", o.code.c_str() ) ;
					} // if
					else {
						fprintf( file, "\n" ) ;
					} // else
					
				} // for
			} // else 	
	
			fclose( file ) ;
		} // OutputSICXE
};

void DoSIC() {
	LexicalAnalysis lex ;
	Lexical_Analysis(lex) ; // 切好token 而且有table567 
	SyntaxAnalysis syn ;
	syn.ReadFile() ;
	syn.ReadFormat() ;
	syn.Copy(lex) ;
	syn.TurnMachineCodeSIC() ;
} // DoSIC

void DoSICXE() {
	LexicalAnalysis lex ;
	Lexical_Analysis(lex) ; // 切好token 而且有table567 
	SyntaxAnalysis syn ;
	syn.ReadFile() ;
	syn.ReadFormat() ;
	syn.Copy(lex) ;
	syn.TurnMachineCodeSICXE() ;
} // DoSICXE

int main() {
	char command = '\0' ;
	bool exit = false ;
	while ( !exit ) {
		cout << "請選擇要執行的程式語言(輸入其他指令即結束):\n1.SIC\n2.SICXE\n:" ;
		cin >> command ;
		if ( command == '1' ) {
			DoSIC() ;
		} // if
		else if ( command == '2' ) {
			DoSICXE() ;
		} // else if
		else {
			exit = true ;
		} // else
	} // while
	
	
} // main()



