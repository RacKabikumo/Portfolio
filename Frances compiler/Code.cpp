#include <iostream>
#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h>
#include <string>
#include <fstream> // 讀檔案 
#include <vector>
#include <sstream> // 10轉8,16 
#include <queue>
#include <stack>
using namespace std; 
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static vector<string> source ;
static string fileName = "" ;


struct Table {
	int type = -1 ;
	int position = -1 ;
};

struct Label {
	string name ;
	int scope ;
	int type = 0 ;
	int t_scope ;
	Table pointer ;
	queue<int> q ;
};



class LexicalAnalysis{
	private:
		vector<string> table1 ;
		vector<string> table2 ;
		string table3[100] ;
		string table4[100] ;
		Label table5[100] ;
		vector<Table> token_done ; // 切好的位置們 
		int scope ;
	public:
	
	void Init() {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table3[i] = "" ;
		} // for
		for ( int j = 0 ; j < 100 ; j++ ) {
			table4[j] = "" ;
		} // for
		Label l ;
		for ( int k = 0 ; k < 100 ; k++ ) {
			table5[k] = l ;
		} // for
	} // Init()
	
	
	void Copy( vector<string> &t1, vector<string> &t2, string t3[], string t4[], Label t5[], vector<Table> &tok  ) {
		t1 = table1 ;
		t2 = table2 ;
		tok = token_done ;
		int i = 0 ;
		for ( i = 0 ; i < 100 ; i++ ) {
			t3[i] = table3[i] ;	
		} // for
		for ( i = 0 ; i < 100 ; i++ ) {
			t4[i] = table4[i] ;
		} // for
		for ( i = 0 ; i < 100 ; i++ ) {
			t5[i] = table5[i] ;
		} // for
	} // copy
	
	bool InputTable() {
		ReadTable( "1" ) ;
		ReadTable( "2" ) ;
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
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( table1.at(i)[0] == word ) {
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
	
	void FindTable1( string temp ) {
		Table tem ;
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( temp == table1.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(1," + t + ")" ;
				WriteFile( word ) ;
				tem.type = 1 ;
				tem.position = i + 1 ;
				token_done.push_back( tem ) ;
			} // if
		} // for
	} // FindTable1
	
	bool FindTable2( string temp ) { // a 97 A 65
	    Table tem ;
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //小寫的字轉大寫 
			name1[j] = temp[j] ;
			if ( temp[j] >= 'a' && temp[j] <= 'z' ) {
				name1[j] = temp[j] - 32 ;
			} // if	
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table2.size() ; i++ ) { // 小寫 
			if ( temp == table2.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(2," + t + ")" ;
				WriteFile( word ) ;
				tem.type = 2 ;
				tem.position = i + 1 ;
				token_done.push_back( tem ) ;
				return true ;
			} // if
			else if ( name == table2.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(2," + t1 + ")" ;
				WriteFile( token ) ;
				tem.type = 2 ;
				tem.position = i + 1 ;
				token_done.push_back( tem ) ;
				return true ;
			} // else if
		} // for
		
		return false ;
	} // FindTable2
	
	void MakeHash( string temp, string t ) {
		bool out = false ;
		int potNum = 0 ;
		for ( int i = 0 ; i < temp.length() && !out ; i++ ) {
			if ( temp[i] >= '0' && temp[i] <= '9' ) {
				;
			} // if
			else if ( temp[i] == '.' && potNum != 1 ) {
				potNum++ ;
			} // else if
			else { // 有非數字的!! symbol!! 
				PutInTable5( temp, t ) ;
				out = true ;
			} // else
		} // for
		
		if ( !out && potNum == 0 ) { // 是數字 
			PutInTable34( temp, table3, "3" ) ;
		} // if
		else if ( !out && potNum == 1 ) { // 浮點數 
			PutInTable34( temp, table4, "4" ) ;
		} // else if
	} // MakeHash
	
	void CheckTable( string temp, string article, int &index, string t ) {
		if ( temp.length() == 1 && IsDelimiter( temp[0] ) ) { // temp只有一個字-->第一個字是delimiter 
			FindTable1( temp ) ; 
		} // if
		else {
			if ( !FindTable2( temp ) ) {
				// 不是table1 2要自己建立 
				MakeHash( temp, t ) ;
			} // if
		} // else
	} // CheckTable
	
	void PutInTable5( string temp, string t ) {
		Table tem ;
		int sum = 0 ;
		for ( int i = 0 ; i < temp.length() ; i++ ) {
			sum = sum + temp[i] ;
		} // for
		bool done = false ;
		int index = sum % 100 ;
		while ( table5[index].name != "" && !done ) {
			if ( index == 99 ) {
				index = 0 ;
			} // if
			else if ( temp == table5[index].name && scope == table5[index].t_scope ) {
				done = true ;
			} // else if
			else {
				if ( ( t == "CALL" || t == "SUBROUTINE" ) && temp == table5[index].name ) { // 除非前一個字是CALL-->CALL不重覆放 
					done = true ;
				} // if
				else {
					index++ ;
				} // else
			} // else 
		} // while
		
		table5[index].name = temp ;
		if ( t == "SUBROUTINE" || t == "PROGRAM" ) { // PROGRAM id之類的要改scope 
			scope = index ;
		} // if
		table5[index].t_scope = scope ;
		char number[3] ;
		itoa( index, number, 10 ) ;
		string t1 = number ;
		string token = "(5," + t1 + ")" ;
		WriteFile( token ) ;
		tem.type = 5 ;
		tem.position = index ;
		token_done.push_back( tem ) ;
	} // PutInTable5
	
	void PutInTable34( string temp, string table[100], string tableNum ) {
		Table tem ;
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
		tem.type = atoi( tableNum.c_str() ) ;
		tem.position = index ;
		token_done.push_back( tem ) ;
	} // PutInTable34
	
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
	
	void CutToken() {
		string temp = "" ;
		string article = "" ;
		int line = 0 ;
		Table tem ;
		string t = "" ;
		while ( line < source.size() ) {
			article = source.at(line) ; // 第line行內容
			WriteFile( article ) ;
			int index = 0 ;
			temp = "" ;
			index = PositionOfWord( article, index ) ; // 先跳過前面的空格
			while ( index != -1 ) { // 此行還沒抓完 
				t = temp ; // CALL用跟算subroutine 
				temp = CatchWord( article, index ) ; // index為temp後一個字 
				// 抓出一個完整的字(在white space或delimiter前的word) index為whitespace或delimiter的位置
				if ( temp != "" ) { // 有東西!  
					CheckTable( temp, article, index, t ) ;
				} // if
				index = PositionOfWord( article, index ) ; // 找這行下一個字的位置
				 
			} // while
			
			WriteFile( "\n" ) ;
			tem.type = -1 ;
			tem.position = -1 ;
			token_done.push_back( tem ) ;
			line++ ; // 換行 

			
			
		}  // while
	} // CutToken()
	
	
}; // LexicalAnalysis()

struct Code {
	Table op1 ;
	Table op2 ;
	Table op3 ;
	Table op4 ;
	string text ;
	int opnum = -1 ;
	string label ;
	bool error = false ;
};

typedef int Int100[100] ;

struct Dimen {
	int id ;
	int arg[10] ;
	int argnum ;
};

struct Symbol {
	int scope ;
	string name ;
};

struct Sub {
	string name ;
	int type ;
	int num ;
};

struct Token {
	Int100 type ;
	Int100 position ;
	int end ;
	bool error = false ;
	string text ;
};

struct Array_Table {
	Table id ;
	Table index ;
};

struct Two {
	Code code ;
	int index = -1 ;
	string text ;
};

struct T0 {
	string name ;
	int type = 0 ;
	int position = -1 ;
};

class Syntax_Analysis {
	private:
		vector<string> table1 ;
		vector<string> table2 ;
		string table3[100] ;
		string table4[100] ;
		Label table5[100] ;
		vector<Code> table6 ;
		vector<int> table7 ;
		vector<Symbol> symtab ; // 存有那些label 
		vector<Symbol> var ; // 存有那些變數 
		vector<Symbol> dimen ;
		vector<Table> token_done ;
		vector<Token> token ;
		vector<Two> two ; // forward 用 
		vector<T0> table0 ;
	public :
		
		void Copy( LexicalAnalysis lex ) {
			lex.Copy( table1, table2, table3, table4, table5, token_done ) ;
		} // Copy
		
		void CreateTokenList() {
			int i = 0 ; // 第幾個token的 
			int index = 0 ; // token這個vector的第幾個 
			int num = 0 ; // 此行的第幾個數 
			Token temp ;
			token.push_back( temp ) ; // 先推個空的 
			while ( i < token_done.size() ) {
				if ( token_done.at(i).type == -1 ) { // 換行 
					token.at(index).end = num ; // 共num個 
					token.push_back( temp ) ;
					index++ ; // 換行嚕 
					num = 0 ;
				} // if
				else {
					token.at(index).type[num] = token_done.at(i).type ;
					token.at(index).position[num] = token_done.at(i).position ;
					num++ ;
				} // else
				i++ ; // 換下一個token 
			} // while
			
			if ( token_done.at(i-1).type == -1 ) {
				token.pop_back() ; // 會多放一個!
			} // if
		} // CreateTokenList
		
		string CheckDatatype( string temp ) {
			int i = 0 ;
			while ( i < temp.length() ) { // 小寫轉大寫 
				if ( temp[i] >= 'a' && temp[i] <= 'z' ) {
					temp[i] = temp[i] - 32 ;
				} // if
				i++ ;
			} // while
			
			if ( temp == "ARRAY" || temp == "CHARACTER" ) {
				return temp ;
			} // if
			else {
				temp = "" ;
				return temp ;
			} // else
		} // CheckDatatype
		
		bool CheckVariable( string temp, int scope ) {
			int i = 0 ;
			Symbol s ;
			while ( i < symtab.size() ) {
				if ( symtab.at(i).name == temp ) {
					if ( symtab.at(i).scope == scope ) {
						return false ;
					} // if
					else { // 名字一樣scope不一樣 
						s.name = temp ;
						s.scope = scope ;
						var.push_back( s ) ;
						return true ;
					} // else
				} // if
				i++ ;
			} // while
			
			s.name = temp ;
			s.scope = scope ;
			var.push_back( s ) ;
			return true ;
		} // CheckVariable
		
		bool CheckSymbol( string temp, int scope ) {
			int i = 0 ;
			Symbol s ;
			while ( i < symtab.size() ) {
				if ( symtab.at(i).name == temp ) {
					if ( symtab.at(i).scope == scope ) {
						return false ;
					} // if
					else { // 名字一樣scope不一樣 
						s.name = temp ;
						s.scope = scope ;
						symtab.push_back( s ) ;
						return true ;
					} // else
				} // if
				i++ ;
			} // while
			
			s.name = temp ;
			s.scope = scope ;
			symtab.push_back( s ) ;
			return true ;
		} // CheckSymbol
		
		void ProgramCheck( Token temp, int line, int &scope ) {
			char number[4] ; // 第幾行-->string 
			itoa( line + 1, number, 10 ) ;
			string num = number ;
			int i = 1 ;
			int subroutine = 0 ;
			if ( i == temp.end ) { // 只有PROGRAM 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : PROGRAM後面沒有接Identifier" ;
			} // if
			else if ( temp.type[i] == 5 ) { // 後面接id! 
				subroutine = i ;
				i++ ;
				if ( i == temp.end ) { // 沒有分號!
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
				else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // id 後面是; 
					i++ ;
					if ( i != temp.end ) { // ;後面還有字!!! 
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
						scope = temp.position[subroutine] ; // 設定scope 
					} // else
				} // else if
				else { // id後面還接了奇怪的東西@@ 
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : Identifier後面接了其他東西" ;
				} // else
			} // else if
			else {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : PROGRAM後面沒有接Identifier" ;
			} // else
		} // ProgramCheck
		
		
		void LabelCheck( Token temp, int line, int scope ) {
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			int i = 1 ;
			int subroutine = 0 ;
			int arr[10] ;
			int index = 0 ;
			string t = "" ;
			if ( i == temp.end ) { // 只有LABEL!! 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : LABEL後面沒有接Identifier" ;
			} // if
			else if ( temp.type[i] == 5 ) { // LABEL id
				subroutine = temp.position[i] ;
				t = table5[subroutine].name ;
				if ( !CheckSymbol( t, scope ) ) { // 重複定義! 
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : LABEL重複宣告了" ;
				} // if
				else {
					arr[index] = subroutine ; // 存有那些id 
					index++ ;
					i++ ;
					if ( i == temp.end ) { // 沒; 
						symtab.pop_back() ;
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // LABEL id ;
						i++ ;
						if ( i != temp.end ) { // ;後面還接東西! 
							symtab.pop_back() ;
							token.at(line).error = true ;
							token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
						} // if
						else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
							table5[subroutine].type = 5 ;
							table5[subroutine].scope = scope ;
						} // else
					} // else if
					else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // LABEL id, 
						while ( i < temp.end ) {
							if ( temp.type[i] == 1 && temp.position[i] == 11 ) {
								i++ ;
								if ( i < temp.end && temp.type[i] == 5 ) {
									subroutine = temp.position[i] ;
									t = table5[subroutine].name ;
									if ( !CheckSymbol( t, scope ) ) { // 重複定義! 
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : LABEL重複宣告了" ;
										for ( int j = 0 ; j < index ; j++ ) {
											symtab.pop_back() ; // 等於前面放的都不要了 
										} // for
										break ;
									} // if
									else {
										arr[index] = subroutine ;
										index++ ;
									} // else
									i++ ;
								} // if
								else {
									for ( int j = 0 ; j < index ; j++ ) {
										symtab.pop_back() ; // 等於前面放的都不要了 
									} // for
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : ,後面出現非Identifier的字" ;
									break ;
								} // else
							} // if
							else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // ;
								i++ ;
								if ( i != temp.end ) {
									for ( int j = 0 ; j < index ; j++ ) {
										symtab.pop_back() ; // 等於前面放的都不要了 
									} // for
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
									break ;
								} // if
								else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
									for ( int j = 0 ; j < index ; j++ ) { // 將對的那些id放好scope跟type 
										subroutine = arr[j] ;
										table5[subroutine].type = 5 ;
										table5[subroutine].scope = scope ;
									} // for
								} // else
							} // else if
							else {
								for ( int j = 0 ; j < index ; j++ ) {
									symtab.pop_back() ; // 等於前面放的都不要了 
								} // for
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : Identifier後面接的不是,也不是;" ;
								break ;
							} // else
						} // while
					} // else if
					else {
						symtab.pop_back() ;
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : Identifier後面沒有接,或;" ;
					} // else
				} // else 
				
			} // else if
			else {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : LABEL後面出現非Identifier的字" ;
			} // else
		} // LabelCheck
		
		void GtoCheck( Token temp, int line, int scope ) {
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			int i = 1 ;
			int subroutine = 0 ;
			if ( i == temp.end ) { // 只有GTO
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : GTO後面沒有接Identifier" ;
			} // if
			else if ( temp.type[i] == 5 ) { // 後面接id! 
				subroutine = temp.position[i] ;
				i++ ;
				if ( i == temp.end ) { // 沒有分號!
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
				else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // id 後面是; 
					i++ ;
					if ( i != temp.end ) { // ;後面還有字!!! 
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
						if ( CheckSymbol( table5[subroutine].name, scope ) ) { // ID不存在!(LABEL) 
							symtab.pop_back() ; // 因為會放進去但是他不是LABEL 
							token.at(line).error = true ;
							token.at(line).text = "line " + num + "  : LABEL尚未被宣告" ;
						} // if
					} // else
				} // else if
				else { // id後面還接了奇怪的東西@@ 
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : Identifier後面接了其他東西" ;
				} // else
			} // else if
			else {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : GTO後面沒有接Identifier" ;
			} // else
		} // GtoCheck
		
		void EndCheck( Token temp, int line ) {
			int i = 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			if ( i == temp.end ) { // 只有ENDP||ENDS 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
			} // if
			else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // ENDP || ENDS ;
				i++ ;
				if ( i != temp.end ) { // ;後面還有東西 
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
			} // else if
			else { // ENDP||ENDS後面接奇怪東西 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 後面接了其他東西" ;
			} // else
		} // EndCheck
		
		int CheckDataType( Token temp, int i, int line ) {
			int position = 0 ;
			string t = "" ;
			int type = 0 ;
			if ( temp.type[i] == 2 ) { // datatype是table2的 
				position = temp.position[i] ;
				if ( position == 2 ) { // Boolean
					type = 2 ;
				} // if
				else if ( position == 14 ) { // Integer
					type = 4 ;
				} // else if
				else if ( position == 15 ) { // LABEL
					type = 5 ;
				} // else if
				else if ( position == 22 ) { // real
					type = 6 ;
				} // else if
			} // if
			else if ( temp.type[i] == 5 ) {
				position = temp.position[i] ;
				t = CheckDatatype( table5[position].name ) ;
				if ( t == "ARRAY" ) {
					type = 1 ;
				} // if
				else if ( t == "CHARACTER" ) {
					type = 3 ;
				} // else if
				else {
					token.at(line).error = true ;
				} // else
			} // else if
			else {
				token.at(line).error = true ;
			} // else
			
			return type ;
		} //  CheckDataType
		
		void VariableCheck( Token temp, int line, int scope ) { // VARIABLE <datatype> : id{,id}
			int i = 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			string t = "" ;
			int position = 0 ;
			int type = 0 ;
			int arr[10] ;
			int index = 0 ;
			bool done = false ;
			if ( i == temp.end ) {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				done = true ;
			} // if
			else {
				type = CheckDataType( temp, i, line ) ;
				if ( type == 0 ) {
					token.at(line).text = "line " + num + "  : VARIABLE後面沒有接datatype" ;
					done = true ;
				} // if
			} // else
			
			if ( !done ) {
				i++ ;
				if ( i == temp.end ) { // 沒有冒號! 
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
				else if ( temp.type[i] == 1 && temp.position[i] == 12 ) { // :
					i++ ;
					if ( i == temp.end ) { // 沒有id! 
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else if ( temp.type[i] == 5 ) { // VARIABLE <datatype> : id
						position = temp.position[i] ;
						t = table5[position].name ;
						if ( !CheckVariable( t, scope ) ) { // 重複定義! 
							token.at(line).error = true ;
							token.at(line).text = "line " + num + "  : 變數重複宣告了" ;
						} // if
						else {
							arr[index] = position ;
							index++ ;
							i++ ;
							if ( i == temp.end ) { // 沒; 
								var.pop_back() ;
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
							} // if
							else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // VARIABLE <datatype> : id ;
								i++ ;
								if ( i != temp.end ) { // ;後面還接東西! 
									var.pop_back() ;
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
								} // if
								else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
									table5[position].scope = scope ;
									table5[position].type = type ;
								} // else
							} // else if
							else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // VARIABLE <datatype> : id, 
								while ( i < temp.end ) {
									if ( temp.type[i] == 1 && temp.position[i] == 11 ) {
										i++ ;
										if ( i < temp.end && temp.type[i] == 5 ) {
											position = temp.position[i] ;
											t = table5[position].name ;
											if ( !CheckVariable( t, scope ) ) { // 重複定義! 
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : 變數重複宣告了" ;
												for ( int j = 0 ; j < index ; j++ ) {
													var.pop_back() ;
												} // for
												break ;
											} // if
											else {
												arr[index] = position ;
												index++ ;
											} // else
											i++ ;
										} // if
										else {
											for ( int j = 0 ; j < index ; j++ ) {
												var.pop_back() ;
											} // for
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : ,後面出現非Identifier的字" ;
											break ;
										} // else
									} // if
									else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // ;
										i++ ;
										if ( i != temp.end ) {
											for ( int j = 0 ; j < index ; j++ ) {
												var.pop_back() ;
											} // for
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
											break ;
										} // if
										else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
											for ( int j = 0 ; j < index ; j++ ) {
												position = arr[j] ;
												table5[position].scope = scope ;
												table5[position].type = type ;
											} // for
										} // else
									} // else if
									else {
										for ( int j = 0 ; j < index ; j++ ) {
											var.pop_back() ;
										} // for
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : Identifier後面出現,和;以外的字" ;
										break ;
									} // else
								} // while
							} // else if
							else {
								var.pop_back() ;
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : Identifier後面出現非,或;的字" ;
							} // else
						} // else
					} // else if
					else {
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 冒號後面出現非Identifier的字" ;
					} // else
				} // else if
				else { // 後面接的不是冒號 
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : Datatype後面接的不是冒號" ;
				} // else
			} // if
			
		} // VariableCheck
		
		
		void CallCheck( Token temp, int line, int scope ) { //CALL id(id{,id}) ; || CALL id(num{,num}) ;
			int i = 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			string t = "" ;
			int position = 0 ;
			int t5_posi = 0 ;
			int arr[10] ;
			int index = 0 ;
			if ( i == temp.end ) { // 只有CALL 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
			} // if
			else if ( temp.type[i] == 5 ) { // CALL id
				position = temp.position[i] ;
				t5_posi = position ;
				t = table5[position].name ;
				i++ ;
				if ( i == temp.end ) { // 只有CALL id
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
				else if ( temp.type[i] == 1 && temp.position[i] == 2 ) { // CALL id(
					i++ ;
					if ( i == temp.end ) { // 只有CALL id(
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else if ( temp.type[i] == 5 ) { // CALL id(id !!!!!!!!!!!!!!!!!!!先只測id的情況!!!!!!!!!!!!!!!!!!!!!!!
						position = temp.position[i] ;
						arr[index] = position ;
						index++ ;
						i++ ;
						if ( i == temp.end ) { // 只有CALL id(id
							token.at(line).error = true ;
							token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
						} // if
						else if ( temp.type[i] == 1 && temp.position[i] == 3 ) { // CALL id(id)
							i++ ;
							if ( i == temp.end ) { // 只有CALL id(id)
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
							} // if
							else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // CALL id(id);
							    i++ ;
								if ( i != temp.end ) { // ;後面有東西 
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : ;後面有東西" ;
								} // if
								else { // 對對對對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  
									table5[t5_posi].pointer.type = 7 ;
									table5[t5_posi].q.push( table7.size() + 1 ) ;
									table7.push_back( index ) ; // 有幾個參數 
									for ( int j = 0 ; j < index ; j++ ) {
										table7.push_back( 5 ) ; 
										table7.push_back( arr[j] ) ; 
									} // for
								} // else
							} // else if
							else {
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : )後面接的不是;" ;
							} // else
						} // else if
						else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // CALL id(id,
							while ( i < temp.end ) {
								if ( temp.type[i] == 1 && temp.position[i] == 11 ) {
									i++ ;
									if ( i < temp.end && temp.type[i] == 5 ) {
										position = temp.position[i] ;
										arr[index] = position ;
										index++ ;
										i++ ;
									} // if
									else {
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : ,後面出現非Identifier的字" ;
										break ;
									} // else
								} // if
								else if ( temp.type[i] == 1 && temp.position[i] == 3 ) { // )
									i++ ;
									if ( i == temp.end ) { // )後面沒; 
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
									} // if
									else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // ;
										i++ ;
										if ( i != temp.end ) {
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
											break ;
										} // if
										else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
											table5[t5_posi].pointer.type = 7 ;
											table5[t5_posi].q.push( table7.size() + 1 ) ;
											table7.push_back( index ) ; // 有幾個參數 
											for ( int j = 0 ; j < index ; j++ ) {
												table7.push_back( 5 ) ; 
												table7.push_back( arr[j] ) ; 
											} // for
										} // else
									} // else if
									else { 
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : )後面接的不是;" ;
										break ;
									} // else
								} // else if
								else {
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : Identifier後面出現,和)以外的字" ;
									break ;
								} // else
							} // while
						} // else if
						else {
							token.at(line).error = true ;
							token.at(line).text = "line " + num + "  : Identifier後面出現,和)以外的字" ;
						} // else
					} // else if
					else {
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : (後面接的不是Identifier" ;
					} // else
				} // else if
				else {
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 第一個Identifier後面沒接(" ;
				} // else
			} // else if
			else {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : CALL後面接的不是Identifier" ;
			} // else
		} // CallCheck
		
		
		
		void SubroutineCheck( Token temp, int line, int &scope ) { // SUBROUTINE id(<datatype>:id{,id});
			int i = 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			string t = "" ;
			int position = 0 ;
			int type = 0 ;
			int t_scope = scope ;
			int arr[10] ;
			int index = 0 ;
			Sub s ;
			if ( i == temp.end ) { // 只有SUBROUTINE 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
			} // if
			else if ( temp.type[i] == 5 ) { // SUBROUTINE id
				position = temp.position[i] ;
				t_scope = position ;
				s.name = table5[position].name ;
				i++ ;
				if ( i == temp.end ) { // 只有SUBROUTINE id
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
				else if ( temp.type[i] == 1 && temp.position[i] == 2 ) { // SUBROUTINE id(
					i++ ;
					if ( i == temp.end ) { // 只有SUBROUTINE id(
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else {
						type = CheckDataType( temp, i, line ) ;
						if ( type == 0 ) { // 後面接的不是datatype 
							token.at(line).text = "line " + num + "  : (後面沒有接datatype" ;
						} // if
						else { //  SUBROUTINE id(<datatype>
							s.type = type ;
							i++ ;
							if ( i == temp.end ) { // 只有SUBROUTINE id(<datatype>
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
							} // if
							else if ( temp.type[i] == 1 && temp.position[i] == 12 ) { // SUBROUTINE id(<datatype>:
								i++ ;
								if ( i == temp.end ) { // 只有SUBROUTINE id(<datatype>:
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
								} // if
								else if ( temp.type[i] == 5 ) { //  SUBROUTINE id(<datatype>:id
									position = temp.position[i] ;
									t = table5[position].name ;
									if ( !CheckVariable( t, t_scope ) ) { // id重複定義 
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : 變數重複宣告了" ;
									} // if
									else {
										arr[index] = position ;
										index++ ;
										i++ ;
										if ( i == temp.end ) { // 只有SUBROUTINE id(<datatype>:id
											var.pop_back() ;
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
										} // if
										else if ( temp.type[i] == 1 && temp.position[i] == 3 ) { // SUBROUTINE id(<datatype>:id)
											i++ ;
											if ( i == temp.end ) { // 只有SUBROUTINE id(<datatype>:id)
												var.pop_back() ;
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
											} // if
											else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // SUBROUTINE id(<datatype>:id);
												i++ ;
												if ( i != temp.end ) { // ;後面還有東西! 
													var.pop_back() ;
													token.at(line).error = true ;
													token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
												} // if
												else { //!!!!!!!!!!!!!!!!!!!!!!!對的!!!!!!!!!!!!!!!!!!!!!!! 
													scope = t_scope ;
													table5[position].scope = scope ;
													table5[position].type = type ;
													s.num = 1 ;
												}  // else
											} // else if
											else { // 
												var.pop_back() ;
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : )後面不是;" ;
											} // else
										} // else if
										else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // SUBROUTINE id(<datatype>:id,
											while ( i < temp.end ) {
												if ( temp.type[i] == 1 && temp.position[i] == 11 ) {
													i++ ;
													if ( i < temp.end && temp.type[i] == 5 ) {
														position = temp.position[i] ;
														t = table5[position].name ;
														if ( !CheckVariable( t, t_scope ) ) { // 重複定義! 
															token.at(line).error = true ;
															token.at(line).text = "line " + num + "  : 變數重複宣告了" ;
															for ( int j = 0 ; j < index ; j++ ) {
																var.pop_back() ;
															} // for
															break ;
														} // if
														else {
															arr[index] = position ;
															index++ ;
														} // else
														i++ ;
													} // if
													else {
														for ( int j = 0 ; j < index ; j++ ) {
															var.pop_back() ;
														} // for
														token.at(line).error = true ;
														token.at(line).text = "line " + num + "  : ,後面出現非Identifier的字" ;
														break ;
													} // else
												} // if
												else if ( temp.type[i] == 1 && temp.position[i] == 3 ) { // )
													i++ ;
													if ( i == temp.end ) { // )後面沒; 
														for ( int j = 0 ; j < index ; j++ ) {
															var.pop_back() ;
														} // for
														token.at(line).error = true ;
														token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
													} // if
													else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // ;
													    i++ ;
														if ( i != temp.end ) {
															for ( int j = 0 ; j < index ; j++ ) {
																var.pop_back() ;
															} // for
															token.at(line).error = true ;
															token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
															break ;
														} // if
														else { // 都對!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
															scope = t_scope ;
															for ( int j = 0 ; j < index ; j++ ) {
																position = arr[j] ;
																table5[position].scope = scope ;
																table5[position].type = type ;
															} // for
															s.num = index ;
														} // else
													} // else if
													else { 
														for ( int j = 0 ; j < index ; j++ ) {
															var.pop_back() ;
														} // for
														token.at(line).error = true ;
														token.at(line).text = "line " + num + "  : )後面接的不是;" ;
														break ;
													} // else
												} // else if
												else {
													for ( int j = 0 ; j < index ; j++ ) {
														var.pop_back() ;
													} // for
													token.at(line).error = true ;
													token.at(line).text = "line " + num + "  : Identifier後面出現,和)以外的字" ;
													break ;
												} // else
											} // while
										} // else if
										else {
											var.pop_back() ;
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : Identifier後面出現,和)以外的字" ;
										} // else
									} // else
								} // else if
								else {
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : :後面不是Identifier" ;
								} // else
							} // else if
							else {
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : Datatype後面沒有:" ;
							} // else
						} // else
					} // else
				} // else if
				else {
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 第一個Identifier後面沒接(" ;
				} // else
			} // else if
			else {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : SUBROUTINE後面沒接Identifier" ;
			} // else
		} // SubroutineCheck
		
		void DimensionCheck( Token temp, int line, int scope ) { // DIMENSION<datatype>:<array>{,<array>}; <array>--> id(number{,number})
			int i = 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			string t = "" ;
			int position = 0 ;
			int type = 0 ;
			Dimen d[10] ; // 幾個array
			Symbol sym ;
			int index = 0 ; 
			int argnum = 0 ;
			bool done = false ;
			if ( i == temp.end ) {
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				done = true ;
			} // if
			else {
				type = CheckDataType( temp, i, line ) ;
				if ( type == 0 ) {
					token.at(line).text = "line " + num + "  : DIMENSION後面沒有接datatype" ;
					done = true ;
				} // if
			} // else
			
			if ( !done ) {
				i++ ;
				if ( i == temp.end ) { // 只有DIMENSION<datatype>
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
				} // if
				else if ( temp.type[i] == 1 && temp.position[i] == 12 ) { // DIMENSION<datatype>:
					i++ ;
					if ( i == temp.end ) {
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
					} // if
					else if ( temp.type[i] == 5 ) { // DIMENSION<datatype> : id
						while ( i < temp.end && token.at(line).error == false ) {
							d[index].id = temp.position[i] ;
							i++ ;
							if ( i == temp.end ) { // 只有 DIMENSION<datatype> : id
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
							} // if
							else if ( temp.type[i] == 1 && temp.position[i] == 2 ) { // DIMENSION<datatype> : id(
								i++ ;
								if ( i == temp.end ) { // 只有 DIMENSION<datatype> : id(
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
								} // if
								else if ( temp.type[i] == 3 ) { // DIMENSION<datatype> : id(number
									d[index].arg[argnum] = temp.position[i] ;
									argnum++ ;
									d[index].argnum = argnum ;
									i++ ;
									if ( i == temp.end ) { // 只有 DIMENSION<datatype> : id(number
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
									} // if
									else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // DIMENSION<datatype> : id(number,
										while ( i < temp.end && token.at(line).error == false ) {
											if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // DIMENSION<datatype> : id(number,
												i++ ;
												if ( i == temp.end ) {
													token.at(line).error = true ;
													token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
												} // if
												else if ( temp.type[i] == 3 ) { // DIMENSION<datatype> : id(number,number
													d[index].arg[argnum] = temp.position[i] ;
													argnum++ ;
													d[index].argnum = argnum ;
													i++ ;
												} // else if
												else {
													token.at(line).error = true ;
													token.at(line).text = "line " + num + "  : ,後面沒接數字" ;
												} // else
											} // if
											else if ( temp.type[i] == 1 && temp.position[i] == 3 ) { // // DIMENSION<datatype> : id(number,number)
												i++ ;
												if ( i == temp.end ) {
													token.at(line).error = true ;
													token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
												} // if
												else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // DIMENSION<datatype> : id(number,number),
													i++ ;
													if ( i == temp.end ) {
														token.at(line).error = true ;
														token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
													} // if
													else if ( temp.type[i] == 5 ) { // DIMENSION<datatype> : id(number,number),id
														index++ ; // 繼續往下跑 
														break ;
													} // else if
												} // else if
												else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // DIMENSION<datatype> : id(number,number);
													i++ ;
													if ( i != temp.end ) {
														token.at(line).error = true ;
														token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
													} // if
													else { ///////////////////////////////////////////////////////
														for ( int k = 0 ; k <= index ; k++ ) {
															position = d[k].id ;
															argnum = d[k].argnum ;
															sym.name = table5[position].name ;
															sym.scope = scope ;
															dimen.push_back( sym ) ;
															table5[position].scope = scope ;
															table5[position].type = type ;
															table5[position].pointer.type = 7 ; 
															table5[position].q.push( table7.size() + 1 ) ;
															table7.push_back( type ) ; // datatype
															table7.push_back( argnum ) ; // 幾維
															for ( int j = 0 ; j < argnum ; j++ ) {
																int t_position = d[k].arg[j] ;
																t_position = atoi(table3[t_position].c_str()) ;
																table7.push_back( t_position ) ;
															} // for
														} // for
													} // else
												} // else if
												else { 
													token.at(line).error = true ;
													token.at(line).text = "line " + num + "  : )後面沒接;或," ;
												} // else
											} // else if
											else {
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : 數字後面沒接,或)" ;
											} // else
										} // while
									} // else if
									else if ( temp.type[i] == 1 && temp.position[i] == 3 ) { // DIMENSION<datatype> : id(number)
										i++ ;
										if ( i == temp.end ) { // 只有 DIMENSION<datatype> : id(number)
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
										} // if
										else if ( temp.type[i] == 1 && temp.position[i] == 11 ) { // DIMENSION<datatype> : id(number),-->有下一個id 
											i++ ;
											if ( i == temp.end ) {
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
											} // if
											else if ( temp.type[i] == 5 ) { // DIMENSION<datatype> : id(number),id
												// 不做事 繼續跑回圈
												index++ ; 
												argnum = 0 ;
											} // else if
											else {
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : ,後面不是Identifier" ;
											} // else
										} // else if
										else if ( temp.type[i] == 1 && temp.position[i] == 1 ) { // DIMENSION<datatype> : id(number) ;
											i++ ;
											if ( i != temp.end ) {
												token.at(line).error = true ;
												token.at(line).text = "line " + num + "  : ;後面還有東西" ;
											} // if
											else {//////////////////////////////////////////////////////////////
												for ( int k = 0 ; k <= index; k++ ) {
													position = d[k].id ;
													argnum = d[k].argnum ;
													sym.name = table5[position].name ;
													sym.scope = scope ;
													dimen.push_back( sym ) ;
													table5[position].scope = scope ;
													table5[position].type = type ;
													table5[position].pointer.type = 7 ;
													table5[position].q.push( table7.size() + 1 ) ;
													table7.push_back( type ) ; // datatype
													table7.push_back( argnum ) ; // 幾維
													for ( int j = 0 ; j < argnum ; j++ ) {
														int t_position = d[k].arg[j] ;
														t_position = atoi(table3[t_position].c_str()) ;
														table7.push_back( t_position ) ;
													} // for
												} // for
											} // else 
										} // else if
										else {
											token.at(line).error = true ;
											token.at(line).text = "line " + num + "  : )後面沒接,或;" ;
										} // else
									} // else if
									else {
										token.at(line).error = true ;
										token.at(line).text = "line " + num + "  : 數字後面沒接,或)" ;
									} // else
								} // else if
								else {
									token.at(line).error = true ;
									token.at(line).text = "line " + num + "  : (後面沒接數字" ;
								} // else
							} // else if
							else {
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : Identifier後面沒接(" ;
							} // else
						} // while
					} // else if
					else { 
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : :後面沒有接Identifier" ;
					} // else
				} // else if
				else {
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : Datatype後面沒有接:" ;
				} // else
			} // if
		} // DimensionCheck
		
		
		void IfCheck( Token temp, int line, int scope ) { // IF 條件 THEN 做事 ELSE 做其他事 ; 
			int i = temp.end - 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			Token t ;
			int index = 0 ;
			if ( temp.type[i] != 1 || temp.position[i] != 1 ) { // 結尾不是; 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
			} // if
			else { // 未完成!!!!!!!!!ㄚㄚㄚㄚㄚㄚㄚㄚㄚㄚㄚ 
				i = 1 ;
				while ( i < temp.end && !( temp.type[i] == 2 && temp.position[i] == 24 ) ) { // != THEN
					if ( i % 2 == 1 && temp.type[i] == 5 || temp.type[i] == 3 || temp.type[i] == 4 ) {
						;
					} // if
					else if ( i % 2 == 0 && temp.type[i] == 2 && ( temp.position[i] == 1 || temp.position[i] == 19 || temp.position[i] == 10 || temp.position[i] == 9 || temp.position[i] == 17 || temp.position[i] == 16 || temp.position[i] == 8 || temp.position[i] == 18 ) ) {
						;
					} // else if
					else {
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 條件文法錯誤!!" ;
					} // else
					i++ ;
				} // while
				if ( !token.at(line).error && i != temp.end ) { // 上面沒錯 
					i++ ;
					while ( i < temp.end && !( temp.type[i] == 2 && temp.position[i] == 5 ) ) { // ELSE
						t.type[index] = temp.type[i] ;
						t.position[index] = temp.position[i] ;
						i++ ;
						index++ ;
					} // while
					t.type[index] = 1 ;
					t.position[index] = 1 ;
					index++ ;
					t.end = index ;
					index = 0 ;
					if ( i == temp.end ) {
						token.at(line).error = true ;
						token.at(line).text = "line " + num + "  : 沒有ELSE" ;
					} // if
					else if ( t.type[index] == 2 && t.position[index] == 11 ) { // GTO id ;
						GtoCheck( t, line, scope ) ;
					} // if
					else if ( t.type[index] == 2 && t.position[index] == 3 ) { // CALL id(id{,id}) ;
						CallCheck( t, line, scope ) ;
					} // else if
					else if ( temp.type[i] == 5 ) { // 可能label開頭
						string str = table5[temp.position[i]].name ; 
						bool haveAssign = false ;
						for ( int j = 0 ; j < temp.end ; j++ ) {
							if ( t.type[j] == 1 && t.position[j] == 4 ) { // 有= 
								haveAssign = true ;
								break ;
							} // if 
						} // for
						
						if ( haveAssign ) {
							AssignCheck( t, line, scope ) ;
						} // if
					} // else if
					
					if ( !token.at(line).error && i != temp.end ) {
						i++ ;
						index = 0 ;
						while ( i < temp.end ) {
							t.type[index] = temp.type[i] ;
							t.position[index] = temp.position[i] ;
							i++ ;
							index++ ;
						} // while
						
						t.end = index ;
						index = 0 ;
						if ( t.type[index] == 2 && t.position[index] == 11 ) { // GTO id ;
							GtoCheck( temp, line, scope ) ;
						} // if
						else if ( t.type[index] == 2 && t.position[index] == 3 ) { // CALL id(id{,id}) ;
							CallCheck( t, line, scope ) ;
						} // else if
						else if ( temp.type[i] == 5 ) { // 可能label開頭
							string str = table5[temp.position[i]].name ; 
							bool haveAssign = false ;
							for ( int j = 0 ; j < temp.end ; j++ ) {
								if ( t.type[j] == 1 && t.position[j] == 4 ) { // 有= 
									haveAssign = true ;
									break ;
								} // if 
							} // for
							
							if ( haveAssign ) {
								AssignCheck( t, line, scope ) ;
							} // if
						} // else if
					} // if
				} // if
				else if ( !token.at(line).error ) {
					token.at(line).error = true ;
					token.at(line).text = "line " + num + "  : 條件後面沒有接THEN" ;
				} // else if
			} // else
		} // IfCheck
		
		
		void AssignCheck( Token temp, int line, int scope ) {
			int i = temp.end - 1 ;
			char number[4] ; // 第幾行-->string 
			itoa( line+1, number, 10 ) ;
			string num = number ;
			if ( temp.type[i] != 1 || temp.position[i] != 1 ) { // 結尾不是; 
				token.at(line).error = true ;
				token.at(line).text = "line " + num + "  : 最後字元非結束指令 ';'" ;
			} // if
			else { // 未完成!!!!!!!!!ㄚㄚㄚㄚㄚㄚㄚㄚㄚㄚㄚ 
				i = 0 ;
				while ( i < temp.end ) {
					if ( temp.type[i] == 5 || temp.type[i] == 3 || temp.type[i] == 4 ) {
						if ( i > 0 && ( temp.type[i-1] == 5 || temp.type[i-1] == 3 || temp.type[i-1] == 4 ) ) {
							token.at(line).error = true ;
							token.at(line).text = "line " + num + "  : 兩個operand中間沒有operator" ;
							break ;
						} // if
						else {
							;
						} // else
					} // if
					else if ( temp.type[i] == 1 ) {
						if ( temp.type[i-1] == 1 ) { // +( || )-
							if ( temp.position[i] == 2 ) { // ( 本身(-->前面可以有operator 
								;
							} // if
							else if ( temp.position[i-1] == 3 ) { // 前一個)-->自己可以是operator 
								;
							} // else if
							else {
								token.at(line).error = true ;
								token.at(line).text = "line " + num + "  : 不能兩個operator連著" ;
								break ;
							} // else
						} // if
					} // else if
					i++ ;
				} // while
			} // else
		} // AssignCheck
		
		void CheckSyntax() {
			int line = 0 ;
			int scope = 0 ;
			int i = 0 ;
			Token temp ;
			bool label = false ;
			while ( line < token.size() ) {
				if ( !label ) { // 有LABEL在第一個要2次 
					temp = token.at(line) ;
				} // if
				if ( temp.type[i] == 2 && temp.position[i] == 21 ) { // Program id ;
					ProgramCheck( temp, line, scope ) ;
					label = false ;
				} // if
				else if ( temp.type[i] == 2 && temp.position[i] == 15 ) { // LABEL id{,id} ; 
					LabelCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && temp.position[i] == 11 ) { // GTO id ;
					GtoCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && ( temp.position[i] == 6 || temp.position[i] == 7 ) ) { // ENDP ; || ENDS ;
					EndCheck( temp, line ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && temp.position[i] == 25 ) { // VARIABLE <datatype> : id{,id}
					VariableCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && temp.position[i] == 3 ) { // CALL id(id{,id}) ;
					CallCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && temp.position[i] == 23 ) { // SUBROUTINE id(<datatype>:id{,id});
					SubroutineCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && temp.position[i] == 4 ) { // DIMENSION<datatype>:<array>{,<array>}; <array>--> id(number{,number})
					DimensionCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 2 && temp.position[i] == 12 ) { // IF
					IfCheck( temp, line, scope ) ;
					label = false ;
				} // else if
				else if ( temp.type[i] == 5 ) { // 可能label開頭
					string str = table5[temp.position[i]].name ; 
					if ( !CheckSymbol( str, scope ) ) { // 已存在 -->有找到 應該是LABEL 
						label = true ;
						line-- ;
						for ( int j = 0 ; j < 99 ; j++ ) { // 去掉LABEL 
							temp.type[j] = temp.type[j+1] ;
							temp.position[j] = temp.position[j+1] ;
						} // for
						temp.end = temp.end - 1 ;
					} // if
					else { // 不存在 
						symtab.pop_back() ;
						bool haveAssign = false ;
						for ( int j = 0 ; j < temp.end ; j++ ) {
							if ( temp.type[j] == 1 && temp.position[j] == 4 ) { // 有= 
								haveAssign = true ;
								break ;
							} // if 
						} // for
						
						if ( haveAssign ) {
							AssignCheck( temp, line, scope ) ;
							label = false ;
						} // if
					} // else
					
				} // else if
				line++ ;
			} // while
		} // CheckSyntax
		
		void StatementIF( Token temp, string str, string label ) {
			int i = 1 ;
			char number[4] ;
			string num = "" ;
			int index = 0 ;
			stack<Table> op_r ; // operator +-*/
			stack<Table> op_d ; // operand XYZ
			T0 t0 ;
			Token t ; // 存要翻的Statement token 
			Table tem ; // push到stack用 
			Table op1 ;
			Table op2 ;
			Table ope ;
			Table clr ;
			Code code ;
			Code clear ;
			int position = 0 ;
			int size = 0 ; // 先存IF的位置 要回來存 
			bool condi = true ;
			while ( i < temp.end ) {
				if ( temp.type[i] == 2 && temp.position[i] == 24 ) { // THEN
					condi = false ; // 條件做完了 
					while ( !op_r.empty() && !op_d.empty() ) {
						op2 = op_d.top() ; // stack 先拿2才拿1 
						op_d.pop() ;
						op1 = op_d.top() ;
						op_d.pop() ;
						ope = op_r.top() ;
						op_r.pop() ; // 取兩個operand 一個operator 
						code.op1.type = ope.type ;
						code.op1.position = ope.position ;
						code.op2.type = op1.type ;
						code.op2.position = op1.position ;
						code.op3.type = op2.type ;
						code.op3.position = op2.position ;
						t0.position = table0.size() ;
						itoa( t0.position+1, number, 10 ) ;
						num = number ;
						t0.name = "T" + num ;
						table0.push_back( t0 ) ;
						code.op4.type = 0 ;
						code.op4.position = table0.size() ; // t = XXX  
						if ( op1.type == 3 ) {
							num = table3[op1.position] ;
						} // if
						else if ( op1.type == 4 ) {
							num = table4[op1.position] ;
						} // else if
						else if ( op1.type == 5 ) {
							num = table5[op1.position].name ;
						} // else if
						else if ( op1.type == 0 ) {
							num = table0.at(op1.position-1).name ;
						} // else if
						code.text = t0.name + " = " + num + " " + table2.at(ope.position-1) + " " ; // table2從1開始 
						if ( op2.type == 3 ) {
							num = table3[op2.position] ;
						} // if
						else if ( op2.type == 4 ) {
							num = table4[op2.position] ;
						} // else if
						else if ( op2.type == 5 ) {
							num = table5[op2.position].name ;
						} // else if
						else if ( op2.type == 0 ) {
							num = table0.at(op2.position-1).name ;
						} // else if
						code.opnum = 4 ;
						code.text = code.text + num ;
						table6.push_back( code ) ;
						//num = FullString( code, table6.size() ) ;
						//WriteFile( num ) ;
						op_d.push( code.op4 ) ;
						code = clear ;
						index = 0 ;
					} // while	
					op1 = op_d.top() ; // IF的op2 
					op_d.pop() ;
					code.op1.type = 2 ;
					code.op1.position = 12 ;
					code.op2.type = op1.type ;
					code.op2.position = op1.position ;
					code.op3.type = 6 ;
					code.op3.position = table6.size() + 2 ; // IF的下一個 從1開始 
					code.opnum = 4 ;
					code.text = "IF " ;
					itoa( code.op3.position, number, 10 ) ;
					num = number ;
					if ( op1.type == 0 ) {
						code.text = "IF " + table0.at(op1.position-1).name + " GO TO " + num + ",ELSE GO TO " ; // table0從0開始放 但要用1開始 
					} // if
					else if ( op1.type == 3 ) {
						code.text = "IF " + table3[op1.position] + " GO TO " + num + ",ELSE GO TO " ;
					} // else if
					else if ( op1.type == 5 ) {
						code.text = "IF " + table5[op1.position].name + " GO TO " + num + ",ELSE GO TO " ;
					} // else if
					size = table6.size() ;
					if ( label != "" ) {
						code.label = label ;
					} // if
					table6.push_back( code ) ; // IF
					code = clear ;
				} // if
				else if ( temp.type[i] == 2 && temp.position[i] == 5 ) { // ELSE
					t.type[index] = 1 ;
					t.position[index] = 1 ; // 放;
					t.end = index + 1 ;
					position = str.find( "THEN" ) ;
					position = position + 4 ;
					for ( int j = position ; j < str.length() ; j++ ) {
						if ( str[j] != ' ' && str[j] != '	' ) {
							position = j ;
							break ;
						} // if
					} // for
					index = str.find( "ELSE" ) ;
					num = str.substr( position, index - position ) + ";" ; // num ;  
					TranslateOneline( t, num ) ; // 翻完之後要繼續翻下一個唷! 下一個就是IF的OP4
					// 如果THEN後面接GTO就要多加一行東西!
					table6.at(size).op4.type = 6 ;
					if ( table6.at(table6.size()-1).op1.position == 11 ) { // 是GTO 
						table6.at(size).op4.position = table6.size() + 1 ; // 從1開始 
						itoa( table6.size() + 1, number, 10 ) ;
					} // if
					else {
						table6.at(size).op4.position = table6.size() + 2 ; // 從1開始 
						itoa( table6.size() + 2, number, 10 ) ;
					} // else
					num = number ;
					table6.at(size).text = table6.at(size).text + num ; // IF那句搞定!
					index = index + 4 ;
					for ( ; index < str.length() ; index++ ) {
						if ( str[index] != ' ' && str[index] != '	' ) {
							break ;
						} // if
					}  // for
					
					num = str.substr( index, str.length() - index ) ; // 後面全部!
					index = 0 ;
					for ( i = i + 1 ; i < temp.end  ; i++ ) {
						t.type[index] = temp.type[i] ;
						t.position[index] = temp.position[i] ;
						index++ ;
					} // for
					t.end = index ;
					if ( table6.at(table6.size()-1).op1.position == 11 ) { // Then後面接GTO 就不用GTO來跳過else 
						size = -1 ; // 表示不用補 
					} // if
					else { // 要跳過else 
						code.op1.type = 2 ; 
						code.op1.position = 11 ; // GTO
						code.op4.type = 6 ;
						size = table6.size() ; // 目前這行的位置 等等翻好else後的statement後要補上 
						code.text = "" ;
						code.opnum = 2 ;
						table6.push_back( code ) ;
					} // else
					TranslateOneline( t, num ) ; // 要翻else後面的statement囉 
					if ( size != -1 ) { // 有要補的 
						table6.at(size).op4.position = table6.size() + 1 ;
						itoa( table6.size() + 1, number, 10 ) ;
						num = number ;
						table6.at(size).text = "GTO " + num ;
					} // if
					code = clear ;
					break ;
				} // else if
				else if ( condi ) {
					if ( temp.type[i] == 5 ) { // operand
						tem.type = 5 ;
						tem.position = temp.position[i] ;
						op_d.push( tem ) ;
					} // if
					else if ( temp.type[i] == 2 ) { // operator
						if ( !op_r.empty() ) {
							op2 = op_d.top() ; // stack 先拿2才拿1 
							op_d.pop() ;
							op1 = op_d.top() ;
							op_d.pop() ;
							ope = op_r.top() ;
							op_r.pop() ; // 取兩個operand 一個operator 
							code.op1.type = ope.type ;
							code.op1.position = ope.position ;
							code.op2.type = op1.type ;
							code.op2.position = op1.position ;
							code.op3.type = op2.type ;
							code.op3.position = op2.position ;
							t0.position = table0.size() ;
							itoa( t0.position+1, number, 10 ) ;
							num = number ;
							t0.name = "T" + num ;
							table0.push_back( t0 ) ;
							code.op4.type = 0 ;
							code.op4.position = table0.size() ; // t = XXX  
							if ( op1.type == 3 ) {
								num = table3[op1.position] ;
							} // if
							else if ( op1.type == 4 ) {
								num = table4[op1.position] ;
							} // else if
							else if ( op1.type == 5 ) {
								num = table5[op1.position].name ;
							} // else if
							else if ( op1.type == 0 ) {
								num = table0.at(op1.position-1).name ;
							} // else if
							code.text = t0.name + " = " + num + " " + table2.at(ope.position-1) + " " ; // table2從1開始 
							if ( op2.type == 3 ) {
								num = table3[op2.position] ;
							} // if
							else if ( op2.type == 4 ) {
								num = table4[op2.position] ;
							} // else if
							else if ( op2.type == 5 ) {
								num = table5[op2.position].name ;
							} // else if
							else if ( op2.type == 0 ) {
								num = table0.at(op2.position-1).name ;
							} // else if
							code.opnum = 4 ;
							code.text = code.text + num ;
							table6.push_back( code ) ;
							//num = FullString( code, table6.size() ) ;
							//WriteFile( num ) ;
							op_d.push( code.op4 ) ;
							code = clear ;
						} // if
						tem.type = 2 ;
						tem.position = temp.position[i] ;
						op_r.push( tem ) ;
					} // else if
					else if ( temp.type[i] == 3 ) { // operand-->數字 
						tem.type = 3 ;
						tem.position = temp.position[i] ;
						op_d.push( tem ) ;
					} // else if
					else if ( temp.type[i] == 4 ) { // operand-->數字 (小數) 
						tem.type = 4 ;
						tem.position = temp.position[i] ;
						op_d.push( tem ) ;
					} // else if
				} // else if
				else if ( !condi ) { // Statement
					t.type[index] = temp.type[i] ;
					t.position[index] = temp.position[i] ;
					index++ ;
				} // else if
				tem = clr ;
				i++ ;
			} // while
		} // StatementIF
		
		bool FindDimen( int position ) {
			for ( int k = 0 ; k < dimen.size() ; k++ ) {
				if ( dimen.at(k).name == table5[position].name ) {
					return true ;
				} // if
			} // for
			return false ;
		} // FindDimen
		
		int Hash( string temp ) {
			int i = 0 ;
			int sum = 0 ;
			while ( i < temp.length() ) {
				sum = sum + temp[i] ;
				i++ ;
			} // while
			
			sum = sum % 100 ;
			bool done = false ;
			while ( !done ) {
				if ( table3[sum] == "" ) {
					table3[sum] = temp ;
					done = true ;
				} // if
				else if ( table3[sum] == temp ) {
					done = true ;
				} // else if
				else if ( sum == 99 ) {
					sum = 0 ;
				} // else if
				else {
					sum++ ;
				} // else
			} // while
			
			return sum ;
		} // Hash
		
		void StatementAssign( Token temp, string str, string label ) {
			int i = 0 ;
			int index = 0 ;
			Code code ;
			Code clear ;
			bool array = false ;
			bool done = false ;
			vector<Array_Table> vec ;
			Array_Table arr ;
			Table tem ;
			Table op1 ; 
			Table op2 ;
			Table ope ;
			T0 t0 ;
			//itoa( table6.size() + 1, number, 10 ) ;
			char number[4] ;
			string num = "" ;
			stack<Table> op_r ; // +-*/
			stack<Table> op_d ; // XYZ
			for ( int j = 0 ; j < temp.end ; j++ ) {
				if ( temp.type[j] == 5 ) {
					if ( table5[temp.position[j]].type == 1 ) {
						array = true ;
					} // if
					else {
						array = FindDimen( temp.position[j] ) ;
						if ( array ) {
							break ;
						} // if
					} // else
				} // if
			} // for
			if ( !array ) { // 沒有array 比較好做 
				while ( i < temp.end - 1 ) { // 不要; 
					if ( temp.type[i] == 5 || temp.type[i] == 3 || temp.type[i] == 4 ) { // operand
						tem.type = temp.type[i] ;
						tem.position = temp.position[i] ;
						op_d.push( tem ) ;
					} // if
					else if ( temp.type[i] == 1 ) { // +-*/()
						if ( temp.position[i] == 3 ) { // ) )在stack外面要pop到看到(為止 
							while ( op_r.top().position != 2 ) {
								op2 = op_d.top() ;
								op_d.pop() ;
								op1 = op_d.top() ;
								op_d.pop() ;
								ope = op_r.top() ;
								op_r.pop() ;
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op1 = ope ;
								code.op2 = op1 ;
								code.op3 = op2 ;
								code.op4.type = 0 ;
								code.op4.position = table0.size() ;
								code.opnum = 4 ;
								code.text = t0.name + " = " ;
								if ( op1.type == 5 ) {
									num = table5[op1.position].name ;
								} // if
								else if ( op1.type == 3 ) {
									num = table3[op1.position] ;
								} // else if
								else if ( op1.type == 4 ) {
									num = table4[op1.position] ;
								} // else if
								else if ( op1.type == 0 ) {
									num = table0.at(op1.position-1).name ;
								} // else if
								code.text = code.text + num + " " + table1.at(ope.position-1) + " " ;
								if ( op2.type == 5 ) {
									num = table5[op2.position].name ;
								} // if
								else if ( op2.type == 3 ) {
									num = table3[op2.position] ;
								} // else if
								else if ( op2.type == 4 ) {
									num = table4[op2.position] ;
								} // else if
								else if ( op2.type == 0 ) {
									num = table0.at(op2.position-1).name ;
								} // else if
								code.text = code.text + num ;
								op_d.push( code.op4 ) ;
								table6.push_back( code ) ;
								code = clear ;
							} // while
							op_r.pop() ; // POP左括號 
						} // if
						else if ( temp.position[i] == 2 ) { // (在外面就push 
							tem.type = temp.text[i] ;
							tem.position = temp.position[i] ;
							op_r.push( tem ) ;
						} // else if
						else {
							tem.type = temp.type[i] ;
							tem.position = temp.position[i] ;
							if ( op_r.empty() || PushOrPop( op_r.top(), tem ) ) { // 要PUSH 
								op_r.push( tem ) ;
							} // if
							else {
								op2 = op_d.top() ;
								op_d.pop() ;
								op1 = op_d.top() ;
								op_d.pop() ;
								ope = op_r.top() ;
								op_r.pop() ;
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op1 = ope ;
								if ( ope.position == 4 ) { // =
									code.op4 = op1 ;
									code.op2 = op2 ;
									op1 = op2 ;
									op2 = code.op4 ;
									code.opnum = 3 ;
									code.text = "" ;
								} // if
								else {
									code.op2 = op1 ;
									code.op3 = op2 ;
									code.op4.type = 0 ;
									code.op4.position = table0.size() ;
									code.text = t0.name + " = " ;
									code.opnum = 4 ;
								} // else
								
								if ( op1.type == 5 ) {
									num = table5[op1.position].name ;
								} // if
								else if ( op1.type == 3 ) {
									num = table3[op1.position] ;
								} // else if
								else if ( op1.type == 4 ) {
									num = table4[op1.position] ;
								} // else if
								else if ( op1.type == 0 ) {
									num = table0.at(op1.position-1).name ;
								} // else if
								code.text = code.text + num + " " + table1.at(ope.position-1) + " " ;
								if ( op2.type == 5 ) {
									num = table5[op2.position].name ;
								} // if
								else if ( op2.type == 3 ) {
									num = table3[op2.position] ;
								} // else if
								else if ( op2.type == 4 ) {
									num = table4[op2.position] ;
								} // else if
								else if ( op2.type == 0 ) {
									num = table0.at(op2.position-1).name ;
								} // else if
								code.text = code.text + num ;
								op_d.push( code.op4 ) ;
								table6.push_back( code ) ;
								code = clear ;
								i-- ;
							} // else
						} // else
					} // else if
					i++ ;
				} // while	
				
				if ( !op_r.empty() ) { // 還有東西 
					while ( !op_r.empty() && !op_d.empty() ) {
						op2 = op_d.top() ;
						op_d.pop() ;
						op1 = op_d.top() ;
						op_d.pop() ;
						ope = op_r.top() ;
						op_r.pop() ;
						itoa( table0.size() + 1, number, 10 ) ;
						num = number ;
						code.op1 = ope ;
						if ( ope.position == 4 ) { // =
							code.op2 = op2 ;
							code.op4 = op1 ;
							code.opnum = 3 ;
							code.text = "" ;
						} // if
						else {
							t0.name = "T" + num ;
							t0.type = 0 ;
							t0.position = table0.size() ;
							table0.push_back(t0) ;
							code.op2 = op1 ;
							code.op3 = op2 ;
							code.op4.type = 0 ;
							code.op4.position = table0.size() ;
							code.text = t0.name + " = " ;
							code.opnum = 4 ;
						} // else
						
						if ( op1.type == 5 ) {
							num = table5[op1.position].name ;
						} // if
						else if ( op1.type == 3 ) {
							num = table3[op1.position] ;
						} // else if
						else if ( op1.type == 4 ) {
							num = table4[op1.position] ;
						} // else if
						else if ( op1.type == 0 ) {
							num = table0.at(op1.position-1).name ;
						} // else if
						code.text = code.text + num + " " + table1.at(ope.position-1) + " " ;
						if ( op2.type == 5 ) {
							num = table5[op2.position].name ;
						} // if
						else if ( op2.type == 3 ) {
							num = table3[op2.position] ;
						} // else if
						else if ( op2.type == 4 ) {
							num = table4[op2.position] ;
						} // else if
						else if ( op2.type == 0 ) {
							num = table0.at(op2.position-1).name ;
						} // else if
						code.text = code.text + num ;
						op_d.push( code.op4 ) ;
						table6.push_back( code ) ;
						code = clear ;
					} // while
				} // if
			} // if
			else {
				string str = "" ;
				int position = 0 ;
				while ( i < temp.end - 1 ) {
					if ( temp.type[i] == 5 || temp.type[i] == 3 || temp.type[i] == 4 ) {
						tem.type = temp.type[i] ;
						tem.position = temp.position[i] ;
						op_d.push( tem ) ;
					} // if
					else if ( temp.type[i] == 1 ) {
						tem.type = 1 ;
						tem.position = temp.position[i] ;
						if ( temp.position[i] == 3 ) { // ) POP到(為止
							op2 = op_d.top() ;
							op_d.pop() ;
							op1 = op_d.top() ;
							op_d.pop() ;
							ope = op_r.top() ;
							op_r.pop() ;
							if ( ope.position == 2 ) { // (-->array
								if ( op1.type != 5 || ( op1.type == 5 && FindDimen( op1.position ) ) ) { // 是array 或非ID 
									tem.type = 8 ; // array用
									tem.position = vec.size() ;
									arr.id = op1 ;
									arr.index = op2 ;
									vec.push_back( arr ) ;
									op_d.push( tem ) ; // 推進去! 
								} // if 
								else { // 不是array(Z)的情況-->去掉()把operand推回去 
									op_d.push( op1 ) ; // 遇到(括號了要推回去
									op_d.push( op2 ) ; 
								} // else
							} // if
							else if ( ope.position == 11 ) { // ,-->二維囉 
								// T1 = J - 1
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op1.type = 1 ;
								code.op1.position = 6 ; // -
								code.op2 = op2 ; // J
								code.op3.type = 3 ;
								code.op3.position = Hash( "1" ) ; // 1-->如果沒有要自己放 
								code.op4.type = 0 ;
								code.op4.position = table0.size() ;
								if ( op2.type == 5 ) {
									code.text = t0.name + " = " + table5[op2.position].name + " - 1" ;
								} // if
								else if ( op2.type == 3 ) {
									code.text = t0.name + " = " + table3[op2.position] + " - 1" ; 
								} // else if
								code.opnum = 4 ;
								table6.push_back( code ) ;
								code = clear ;
								// T2 = T1 * M
								code.op1.type = 1 ;
								code.op1.position = 7 ; // *
								code.op2.type = 0 ;
								code.op2.position = table0.size() ;
								code.text = " = " + t0.name + " * " ; // = T1 *
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op4.type = 0 ;
								code.op4.position = table0.size() ;
								code.text = t0.name + code.text ; // T2 = T1 * 
								tem = op_d.top() ; // 拿出array的ID
								index = table5[tem.position].q.front() ; // array的ID的table7位置 (已經+1) 
								// M的位置(table7)
								itoa( index, number, 10 ) ;
								num = number ;
								code.op3.type = 3 ;
								code.op3.position = Hash( num ) ; // M(數字)-->如果沒有要自己放
								code.text = code.text + num ; // T2 = T1 * M
								code.opnum = 4 ;
								table6.push_back( code ) ;
								code = clear ;
								// T3 = T2 + I -->位置get
								code.op2.type = 0 ;
								code.op2.position = table0.size() ; // T2
								if ( op1.type == 5 ) {
									code.text = " = " + t0.name + " + " + table5[op1.position].name ; // = T2 + I
								} // if
								else if ( op1.type == 3 ) {
									code.text = " = " + t0.name + " + " + table3[op1.position] ; // = T2 + I
								} // else if
								code.op3 = op1 ; // I
								code.op1.type = 1 ;
								code.op1.position = 5 ; // +
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op4.type = 0 ;
								code.op4.position = table0.size() ; // T3
								code.opnum = 4 ;
								code.text = t0.name + code.text ; // T3 = T2 + I
								table6.push_back( code ) ;
								//op_d.push( code.op4 ) ; // T3推進去! 
								code = clear ;
								op1 = op_d.top() ; // 拿array id -->B
								op_d.pop() ;
								op_r.pop() ; // 拿掉( 
								op2.type = 0  ;
								op2.position = table0.size() ; // T3
								tem.type = 8 ; // array用
								tem.position = vec.size() ;
								arr.id = op1 ;
								arr.index = op2 ;
								vec.push_back( arr ) ;
								op_d.push( tem ) ; // 推進去! 
								// T4 = B(T3)
								/*code.op1.type = 1 ;
								code.op1.position = 4 ; // =
								op2 = op_d.top() ; // 拿array id -->B
								op_d.pop() ;
								op_r.pop() ; // 拿掉( 
								code.op2 = op2 ;
								code.op3.type = 0  ;
								code.op3.position = table0.size() ; // T3
								code.text = " = " + table5[op2.position].name + "(" + t0.name + ")" ;
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op4.type = 0 ;
								code.op4.position = table0.size() ; // T4
								code.text = t0.name + code.text ;
								code.opnum = 4 ;
								op_d.push( code.op4 ) ;
								table6.push_back( code ) ;
								code = clear ;*/
							} // else if
							else {
								while ( ope.position != 2 ) { // 直到遇到左括號 
									if ( ope.position == 4 ) { // =
										if ( op2.type == 8 ) { // XX = array
											code.op2 = vec.at( op2.position ).id ; // A(k)的A 
											code.op3 = vec.at( op2.position ).index ; // k
											itoa( table0.size() + 1, number, 10 ) ;
											num = number ;
											t0.name = "T" + num ;
											t0.type = 0 ;
											t0.position = table0.size() ;
											table0.push_back(t0) ;
											code.op4.type = 0 ;
											code.op4.position = table0.size() ; // T
											code.op1 = ope ; // =
											if ( code.op3.type == 5 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
											} // if
											else if ( code.op3.type == 3 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(3)
											} // else if
											else if ( code.op3.type == 0 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
											} // else if
											code.opnum = 4 ;
											table6.push_back( code ) ;
											op2 = code.op4 ; // 繼續做
											code = clear ;
										} // if
										if ( op1.type == 8 ) {
											code.op1 = ope ;
											code.op2 = op2 ;
											code.op3 = vec.at( op1.position ).id ;
											code.op4 = vec.at( op1.position ).index ;
											code.opnum = 4 ;
											if ( code.op4.type == 5 ) {
												code.text = table5[code.op3.position].name + "(" + table5[code.op4.position].name + ") = " ;
											} // if
											else if ( code.op4.type == 3 ) {
												code.text = table5[code.op3.position].name + "(" + table3[code.op4.position] + ") = " ;
											} // else if
											else if ( code.op4.type == 0 ) {
												code.text = table5[code.op3.position].name + "(" + table0.at(code.op4.position-1).name + ") = " ;
											} // else if
											if ( op2.type == 0 ) {
												itoa( op2.position, number, 10 ) ;
												num = number ;
												code.text = code.text + "T" + num ;
											}  // if
											else if ( op2.type == 5 ) {
												code.text = code.text + table5[op2.position].name ;
											} // else if
											else if ( op2.type == 3 ) {
												code.text = code.text + table3[op2.position] ;
											} // else if
											else if ( op2.type == 4 ) {
												code.text = code.text + table4[op2.position] ;
											} // else if
											table6.push_back( code ) ;
											code = clear ;
											done = true ;
											i-- ;
										} // if
										else {
											code.op4 = op1 ;
											code.op2 = op2 ;
											code.op1 = ope ;
											//op1 = op2 ;
											//op2 = code.op4 ;
											code.opnum = 3 ;
											code.text = "" ;
										} // else
									} // if
									else {
										if ( op2.type == 8 ) { // XX + array
											code.op2 = vec.at( op2.position ).id ; // A(k)的A 
											code.op3 = vec.at( op2.position ).index ; // k
											itoa( table0.size() + 1, number, 10 ) ;
											num = number ;
											t0.name = "T" + num ;
											t0.type = 0 ;
											t0.position = table0.size() ;
											table0.push_back(t0) ;
											code.op4.type = 0 ;
											code.op4.position = table0.size() ; // T
											code.op1 = ope ; // =
											if ( code.op3.type == 5 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
											} // if
											else if ( code.op3.type == 3 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table3[code.op3.position] + ")" ; // T = A(3)
											} // else if
											else if ( code.op3.type == 0 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
											} // else if
											code.opnum = 4 ;
											table6.push_back( code ) ;
											op2 = code.op4 ; // 繼續做
											code = clear ;
										} // if
										if ( op1.type == 8 ) {
											code.op2 = vec.at( op1.position ).id ; // A(k)的A 
											code.op3 = vec.at( op1.position ).index ; // k
											itoa( table0.size() + 1, number, 10 ) ;
											num = number ;
											t0.name = "T" + num ;
											t0.type = 0 ;
											t0.position = table0.size() ;
											table0.push_back(t0) ;
											code.op4.type = 0 ;
											code.op4.position = table0.size() ; // T
											code.op1.type = 1 ; // =
											code.op1.position = 4 ;
											if ( code.op3.type == 5 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
											} // if
											else if ( code.op3.type == 3 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table3[code.op3.position] + ")" ; // T = A(3)
											} // else if
											else if ( code.op3.type == 0 ) {
												code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
											} // else if
											code.opnum = 4 ;
											table6.push_back( code ) ;
											op1 = code.op4 ; // 繼續做
											code = clear ;
										} // if
										code.op1 = ope ;
										code.op2 = op1 ;
										code.op3 = op2 ;
										itoa( table0.size() + 1, number, 10 ) ;
										num = number ;
										t0.name = "T" + num ;
										t0.type = 0 ;
										t0.position = table0.size() ;
										table0.push_back(t0) ;
										code.op4.type = 0 ;
										code.op4.position = table0.size() ;
										code.text = t0.name + " = " ;
										code.opnum = 4 ;
									} // else
									
									if ( !done ) {
										if ( op1.type == 5 ) {
											num = table5[op1.position].name ;
										} // if
										else if ( op1.type == 3 ) {
											num = table3[op1.position] ;
										} // else if
										else if ( op1.type == 4 ) {
											num = table4[op1.position] ;
										} // else if
										else if ( op1.type == 0 ) {
											num = table0.at(op1.position-1).name ;
										} // else if
										code.text = code.text + num + " " + table1.at(ope.position-1) + " " ;
										if ( op2.type == 5 ) {
											num = table5[op2.position].name ;
										} // if
										else if ( op2.type == 3 ) {
											num = table3[op2.position] ;
										} // else if
										else if ( op2.type == 4 ) {
											num = table4[op2.position] ;
										} // else if
										else if ( op2.type == 0 ) {
											num = table0.at(op2.position-1).name ;
										} // else if
										code.text = code.text + num ;
										op_d.push( code.op4 ) ;
										table6.push_back( code ) ;
										code = clear ;
									} // if
									op2 = op_d.top() ;
									op_d.pop() ;
									op1 = op_d.top() ;
									op_d.pop() ;
									ope = op_r.top() ;
									op_r.pop() ;	
								} // while
								op_d.push( op1 ) ; // 遇到(括號了要推回去
								op_d.push( op2 ) ; 
							} // else			
						} // if
						else if ( temp.position[i] == 2 ) { // (-->push 
							op_r.push( tem ) ;
						} // else if
						else if ( op_r.empty() || PushOrPop( op_r.top(), tem ) ) { // 要PUSH 
							op_r.push( tem ) ;
						} // else if
						else {
							op2 = op_d.top() ;
							op_d.pop() ;
							op1 = op_d.top() ;
							op_d.pop() ;
							ope = op_r.top() ;
							op_r.pop() ;
							if ( ope.position == 4 ) { // =
								if ( op2.type == 8 ) { // XX = array
									code.op2 = vec.at( op2.position ).id ; // A(k)的A 
									code.op3 = vec.at( op2.position ).index ; // k
									itoa( table0.size() + 1, number, 10 ) ;
									num = number ;
									t0.name = "T" + num ;
									t0.type = 0 ;
									t0.position = table0.size() ;
									table0.push_back(t0) ;
									code.op4.type = 0 ;
									code.op4.position = table0.size() ; // T
									code.op1 = ope ; // =
									if ( code.op3.type == 5 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
									} // if
									else if ( code.op3.type == 3 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(3)
									} // else if
									else if ( code.op3.type == 0 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
									} // else if
									code.opnum = 4 ;
									table6.push_back( code ) ;
									op2 = code.op4 ; // 繼續做
									code = clear ;
								} // if
								if ( op1.type == 8 ) {
									code.op1 = ope ;
									code.op2 = op2 ;
									code.op3 = vec.at( op1.position ).id ;
									code.op4 = vec.at( op1.position ).index ;
									code.opnum = 4 ;
									if ( code.op4.type == 5 ) {
										code.text = table5[code.op3.position].name + "(" + table5[code.op4.position].name + ") = " ;
									} // if
									else if ( code.op4.type == 3 ) {
										code.text = table5[code.op3.position].name + "(" + table3[code.op4.position] + ") = " ;
									} // else if
									else if ( code.op4.type == 0 ) {
										code.text = table5[code.op3.position].name + "(" + table0.at(code.op4.position-1).name + ") = " ;
									} // else if
									if ( op2.type == 0 ) {
										itoa( op2.position, number, 10 ) ;
										num = number ;
										code.text = code.text + "T" + num ;
									}  // if
									else if ( op2.type == 5 ) {
										code.text = code.text + table5[op2.position].name ;
									} // else if
									else if ( op2.type == 3 ) {
										code.text = code.text + table3[op2.position] ;
									} // else if
									else if ( op2.type == 4 ) {
										code.text = code.text + table4[op2.position] ;
									} // else if
									table6.push_back( code ) ;
									code = clear ;
									done = true ;
									i-- ;
								} // if
								else {
									code.op4 = op1 ;
									code.op2 = op2 ;
									code.op1 = ope ;
									//op1 = op2 ;
									//op2 = code.op4 ;
									code.opnum = 3 ;
									code.text = "" ;
								} // else
							} // if
							else {
								if ( op2.type == 8 ) { // XX + array
									code.op2 = vec.at( op2.position ).id ; // A(k)的A 
									code.op3 = vec.at( op2.position ).index ; // k
									itoa( table0.size() + 1, number, 10 ) ;
									num = number ;
									t0.name = "T" + num ;
									t0.type = 0 ;
									t0.position = table0.size() ;
									table0.push_back(t0) ;
									code.op4.type = 0 ;
									code.op4.position = table0.size() ; // T
									code.op1 = ope ; // =
									if ( code.op3.type == 5 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
									} // if
									else if ( code.op3.type == 3 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table3[code.op3.position] + ")" ; // T = A(3)
									} // else if
									else if ( code.op3.type == 0 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
									} // else if
									code.opnum = 4 ;
									table6.push_back( code ) ;
									op2 = code.op4 ; // 繼續做
									code = clear ;
								} // if
								if ( op1.type == 8 ) {
									code.op2 = vec.at( op1.position ).id ; // A(k)的A 
									code.op3 = vec.at( op1.position ).index ; // k
									itoa( table0.size() + 1, number, 10 ) ;
									num = number ;
									t0.name = "T" + num ;
									t0.type = 0 ;
									t0.position = table0.size() ;
									table0.push_back(t0) ;
									code.op4.type = 0 ;
									code.op4.position = table0.size() ; // T
									code.op1.type = 1 ; // =
									code.op1.position = 4 ;
									if ( code.op3.type == 5 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
									} // if
									else if ( code.op3.type == 3 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table3[code.op3.position] + ")" ; // T = A(3)
									} // else if
									else if ( code.op3.type == 0 ) {
										code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
									} // else if
									code.opnum = 4 ;
									table6.push_back( code ) ;
									op1 = code.op4 ; // 繼續做
									code = clear ;
								} // if
								code.op1 = ope ;
								code.op2 = op1 ;
								code.op3 = op2 ;
								itoa( table0.size() + 1, number, 10 ) ;
								num = number ;
								t0.name = "T" + num ;
								t0.type = 0 ;
								t0.position = table0.size() ;
								table0.push_back(t0) ;
								code.op4.type = 0 ;
								code.op4.position = table0.size() ;
								code.text = t0.name + " = " ;
								code.opnum = 4 ;
							} // else
							
							if ( !done ) {
								if ( op1.type == 5 ) {
									num = table5[op1.position].name ;
								} // if
								else if ( op1.type == 3 ) {
									num = table3[op1.position] ;
								} // else if
								else if ( op1.type == 4 ) {
									num = table4[op1.position] ;
								} // else if
								else if ( op1.type == 0 ) {
									num = table0.at(op1.position-1).name ;
								} // else if
								code.text = code.text + num + " " + table1.at(ope.position-1) + " " ;
								if ( op2.type == 5 ) {
									num = table5[op2.position].name ;
								} // if
								else if ( op2.type == 3 ) {
									num = table3[op2.position] ;
								} // else if
								else if ( op2.type == 4 ) {
									num = table4[op2.position] ;
								} // else if
								else if ( op2.type == 0 ) {
									num = table0.at(op2.position-1).name ;
								} // else if
								code.text = code.text + num ;
								op_d.push( code.op4 ) ;
								table6.push_back( code ) ;
								code = clear ;
								i-- ;
							} // if
						} // else
					} // else
					i++ ;
				}  // while
				
				while ( !op_r.empty() && !op_d.empty() ) {
					done = false ;
					op2 = op_d.top() ;
					op_d.pop() ;
					op1 = op_d.top() ;
					op_d.pop() ;
					ope = op_r.top() ;
					op_r.pop() ;
					if ( ope.position == 4 ) { // =
						if ( op2.type == 8 ) { // XX = array
							code.op2 = vec.at( op2.position ).id ; // A(k)的A 
							code.op3 = vec.at( op2.position ).index ; // k
							itoa( table0.size() + 1, number, 10 ) ;
							num = number ;
							t0.name = "T" + num ;
							t0.type = 0 ;
							t0.position = table0.size() ;
							table0.push_back(t0) ;
							code.op4.type = 0 ;
							code.op4.position = table0.size() ; // T
							code.op1 = ope ; // =
							if ( code.op3.type == 5 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
							} // if
							else if ( code.op3.type == 3 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(3)
							} // else if
							else if ( code.op3.type == 0 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
							} // else if
							code.opnum = 4 ;
							table6.push_back( code ) ;
							op2 = code.op4 ; // 繼續做
							code = clear ;
						} // if
						if ( op1.type == 8 ) {
							code.op1 = ope ;
							code.op2 = op2 ;
							code.op3 = vec.at( op1.position ).id ;
							code.op4 = vec.at( op1.position ).index ;
							code.opnum = 4 ;
							if ( code.op4.type == 5 ) {
								code.text = table5[code.op3.position].name + "(" + table5[code.op4.position].name + ") = " ;
							} // if
							else if ( code.op4.type == 3 ) {
								code.text = table5[code.op3.position].name + "(" + table3[code.op4.position] + ") = " ;
							} // else if
							else if ( code.op4.type == 0 ) {
								code.text = table5[code.op3.position].name + "(" + table0.at(code.op4.position-1).name + ") = " ;
							} // else if
							if ( op2.type == 0 ) {
								itoa( op2.position, number, 10 ) ;
								num = number ;
								code.text = code.text + "T" + num ;
							}  // if
							else if ( op2.type == 5 ) {
								code.text = code.text + table5[op2.position].name ;
							} // else if
							else if ( op2.type == 3 ) {
								code.text = code.text + table3[op2.position] ;
							} // else if
							else if ( op2.type == 4 ) {
								code.text = code.text + table4[op2.position] ;
							} // else if
							table6.push_back( code ) ;
							code = clear ;
							done = true ;
						} // if
						else {
							code.op4 = op1 ;
							code.op2 = op2 ;
							code.op1 = ope ;
							//op1 = op2 ;
							//op2 = code.op4 ;
							code.opnum = 3 ;
							code.text = "" ;
						} // else
					} // if
					else {
						if ( op2.type == 8 ) { // XX + array
							code.op2 = vec.at( op2.position ).id ; // A(k)的A 
							code.op3 = vec.at( op2.position ).index ; // k
							itoa( table0.size() + 1, number, 10 ) ;
							num = number ;
							t0.name = "T" + num ;
							t0.type = 0 ;
							t0.position = table0.size() ;
							table0.push_back(t0) ;
							code.op4.type = 0 ;
							code.op4.position = table0.size() ; // T
							code.op1.type = 1 ; // =
							code.op1.position = 4 ;
							if ( code.op3.type == 5 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
							} // if
							else if ( code.op3.type == 3 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table3[code.op3.position] + ")" ; // T = A(3)
							} // else if
							else if ( code.op3.type == 0 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
							} // else if
							code.opnum = 4 ;
							table6.push_back( code ) ;
							op2 = code.op4 ; // 繼續做
							code = clear ;
						} // if
						if ( op1.type == 8 ) {
							code.op2 = vec.at( op1.position ).id ; // A(k)的A 
							code.op3 = vec.at( op1.position ).index ; // k
							itoa( table0.size() + 1, number, 10 ) ;
							num = number ;
							t0.name = "T" + num ;
							t0.type = 0 ;
							t0.position = table0.size() ;
							table0.push_back(t0) ;
							code.op4.type = 0 ;
							code.op4.position = table0.size() ; // T
							code.op1.type = 1 ; // =
							code.op1.position = 4 ;
							if ( code.op3.type == 5 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table5[code.op3.position].name + ")" ; // T = A(K)
							} // if
							else if ( code.op3.type == 3 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table3[code.op3.position] + ")" ; // T = A(3)
							} // else if
							else if ( code.op3.type == 0 ) {
								code.text = t0.name + " = " + table5[code.op2.position].name + "(" + table0.at(code.op3.position-1).name + ")" ; // T = A(TX)
							} // else if
							code.opnum = 4 ;
							table6.push_back( code ) ;
							op1 = code.op4 ; // 繼續做
							code = clear ;
						} // if
						code.op1 = ope ;
						code.op2 = op1 ;
						code.op3 = op2 ;
						itoa( table0.size() + 1, number, 10 ) ;
						num = number ;
						t0.name = "T" + num ;
						t0.type = 0 ;
						t0.position = table0.size() ;
						table0.push_back(t0) ;
						code.op4.type = 0 ;
						code.op4.position = table0.size() ;
						code.text = t0.name + " = " ;
						code.opnum = 4 ;
					} // else
					
					if ( !done ) {
						if ( op1.type == 5 ) {
							num = table5[op1.position].name ;
						} // if
						else if ( op1.type == 3 ) {
							num = table3[op1.position] ;
						} // else if
						else if ( op1.type == 4 ) {
							num = table4[op1.position] ;
						} // else if
						else if ( op1.type == 0 ) {
							num = table0.at(op1.position-1).name ;
						} // else if
						code.text = code.text + num + " " + table1.at(ope.position-1) + " " ;
						if ( op2.type == 5 ) {
							num = table5[op2.position].name ;
						} // if
						else if ( op2.type == 3 ) {
							num = table3[op2.position] ;
						} // else if
						else if ( op2.type == 4 ) {
							num = table4[op2.position] ;
						} // else if
						else if ( op2.type == 0 ) {
							num = table0.at(op2.position-1).name ;
						} // else if
						code.text = code.text + num ;
						op_d.push( code.op4 ) ;
						table6.push_back( code ) ;
						code = clear ;
						i-- ;
					} // if
				} // if
			} // else
			
		} // StatementAssign
		
		bool PushOrPop( Table sta, Table tem ) { // true-->push
			if ( tem.position == 9 ) { // ^-->外面是最大的 
				return true ;
			} // if
			else if ( tem.position == 7 || tem.position == 8 ) { // * /
				if ( sta.position == 7 || sta.position == 8 || sta.position == 9 ) { // top>=外面要pop 
					return false ;
				} // if
				else {
					return true ;
				}  // else
			} // else if
			else if ( tem.position == 5 || tem.position == 6 ) { // + -
				if ( sta.position == 5 || sta.position == 6 || sta.position == 7 || sta.position == 8 || sta.position == 9 ) { // top>=外面要pop 
					return false ;
				} // if
				else {
					return true ;
				}  // else
			} // else if
			else if ( tem.position == 4 ) { // =
				return true ;
			} // else if
		} // PushOrPop
		
		void TranslateOneline( Token temp, string str ) {
			int i = 0 ;
			int position = 0 ;
			int index = 0 ;
			Two two_pass ;
			Two clear_two ;
			Code code ;
			Code clear ; // 清空code用 
			bool done = false ;
			bool find = false ;
			string t = "" ;
			while ( !done ) {
				if ( temp.type[0] == 2 && temp.position[0] == 21 ) { // Program id ; --> 不翻 
					done = true ;
				} // if
				else if ( temp.type[0] == 2 && temp.position[0] == 15 ) { // LABEL id{,id} ; 
					i = 1 ; // id的位置 
					while ( i < temp.end ) {
						if ( temp.type[i] == 5 ) { // 不是, 
							position = temp.position[i] ;
							code.text = table5[position].name ;
							code.op1.type = 5 ;
							code.op1.position = position ;
							code.opnum = 1 ;
							table6.push_back( code ) ;
							//t = FullString( code, table6.size() ) ;
							//WriteFile( t ) ;
							code = clear ; // 初始化 
						} // if
						i++ ; 
					} // while
					done = true ;
				} // else if
				else if ( temp.type[0] == 2 && temp.position[0] == 11 ) { // GTO id ; !!!!!!!!!!!!!!!!!!
					for ( i = 0 ; i < str.length() ; i++ ) {
						if ( str[i] != ' ' && str[i] != '	' ) {
							break ;
						} // if
					} // for
					position = str.find( ";" ) ;
					code.text = str.substr( i, position - i ) ;
					i = 0 ;
					position = temp.position[i] ;
					code.op1.type = 2 ;
					code.op1.position = position ;
					i++ ;
					position = temp.position[i] ; // id的位置 
					t = table5[position].name ;
					for ( int j = 0 ; j < table6.size() ; j++ ) {
						if ( t == table6.at(j).label ) {
							position = j + 1 ; // table6從1開始 
							find = true ;
						} // if
					} // for
					code.opnum = 2 ;
					if ( !find ) { // 還沒放LABEL 
						two_pass.code = code ;
						two_pass.index = table6.size() ;
						two_pass.text = t ;
						two.push_back( two_pass ) ;
						two_pass = clear_two ;
					} // if
					else { // LABEL有放了 
						code.op4.type = 6 ;
						code.op4.position = position ;
						find = false ;
					} // else
					table6.push_back( code ) ;
					//t = FullString( code, table6.size() ) ;
					//WriteFile( t ) ;
					code = clear ; // 初始化
					done = true ;
				} // else if
				else if ( temp.type[0] == 2 && ( temp.position[0] == 6 || temp.position[0] == 7 ) ) { // ENDP ; || ENDS ;
					for ( i = 0 ; i < str.length() ; i++ ) {
						if ( str[i] != ' ' && str[i] != '	' ) {
							break ;
						} // if
					} // for
					position = str.find( ";" ) ;
					code.text = str.substr( i, position - i ) ;
					i = 0 ;
					position = temp.position[i] ;
					code.op1.type = 2 ;
					code.op1.position = position ;
					code.opnum = 1 ;
					table6.push_back( code ) ;
					//t = FullString( code, table6.size() ) ;
					//WriteFile( t ) ;
					code = clear ; // 初始化 
					done = true ;
				} // else if
				else if ( temp.type[0] == 2 && temp.position[0] == 25 ) { // VARIABLE <datatype> : id{,id}
					i = 3 ; // id的位置 
					while ( i < temp.end ) {
						if ( temp.type[i] == 5 ) { // 不是, 
							position = temp.position[i] ;
							code.text = table5[position].name ;
							code.op1.type = 5 ;
							code.op1.position = position ;
							code.opnum = 1 ;
							table6.push_back( code ) ;
							//t = FullString( code, table6.size() ) ;
							//WriteFile( t ) ;
							code = clear ; // 初始化 
						} // if
						i++ ; 
					} // while
					done = true ;
				} // else if
				else if ( temp.type[0] == 2 && temp.position[0] == 3 ) { // CALL id(id{,id}) ;
					i = 0 ;
					for ( i = 0 ; i < str.length() ; i++ ) {
						if ( str[i] != ' ' && str[i] != '	' ) {
							break ;
						} // if
					} // for
					position = str.find( ";" ) ;
					code.text = str.substr( i, position - i ) ; // 不要拿; 
					i = 0 ;
					position = temp.position[i] ;
					code.op1.type = 2 ;
					code.op1.position = position ;
					i++ ;
					position = temp.position[i] ;
					code.op2.type = 5 ;
					code.op2.position = position ;
					i++ ;
					index = table5[position].q.front() ;
					table5[position].q.pop() ;
					code.op4.type = 7 ;
					code.op4.position = index ;
					code.opnum = 3 ;
					table6.push_back( code ) ;
					//t = FullString( code, table6.size() ) ;
					//WriteFile( t ) ;
					code = clear ; // 初始化
					done = true ;
				} // else if
				else if ( temp.type[0] == 2 && temp.position[0] == 23 ) { // SUBROUTINE id(<datatype>:id{,id});
					i = 5 ;
					while ( i < temp.end ) {
						if ( temp.type[i] == 5 ) { // 不是, 
							position = temp.position[i] ;
							code.text = table5[position].name ;
							code.op1.type = 5 ;
							code.op1.position = position ;
							code.opnum = 1 ;
							table6.push_back( code ) ;
							//t = FullString( code, table6.size() ) ;
							//WriteFile( t ) ;
							code = clear ; // 初始化 
						} // if
						i++ ; 
					} // while
					done = true ; // 不翻 
				} // else if
				else if ( temp.type[0] == 2 && temp.position[0] == 4 ) { // DIMENSION<datatype>:<array>{,<array>}; <array>--> id(number{,number})
					i = 3 ; // id的位置 
					while ( i < temp.end ) {
						if ( temp.type[i] == 5 ) { // id
							position = temp.position[i] ;
							code.text = table5[position].name ;
							code.op1.type = 5 ;
							code.op1.position = position ;
							code.opnum = 1 ;
							table6.push_back( code ) ;
							//t = FullString( code, table6.size() ) ;
							//WriteFile( t ) ;
							code = clear ; // 初始化 
						} // if
						i++ ; 
					} // while
					done = true ;
				} // else if
				else if ( temp.type[0] == 2 && temp.position[0] == 12 ) { // IF
					StatementIF( temp, str, code.label ) ;
					code = clear ; // 初始化 
					done = true ;
				} // else if
				else if ( temp.type[0] == 5 ) { // 可能label開頭 這裡也要處理assignment !!!!!!!!!!!!!!!!!!!!
					t =  table5[temp.position[0]].name ;
					int scope = table5[temp.position[0]].scope ;
					if ( !CheckSymbol( t, scope ) ) { // 已存在 -->有找到 應該是LABEL 
						code.label = t ; // 存下這裡有的label名
						position = t.length() ;
						while ( position < str.length() ) {
							if ( str[position] != ' ' && str[position] != '	' ) {
								break ;
							} // if
							position++ ;
						}  // while
						str = str.substr( position, str.length() - position ) ; // 拿LABEL後面的東西
						for ( int j = 0 ; j < 99 ; j++ ) { // 去掉LABEL 
							temp.type[j] = temp.type[j+1] ;
							temp.position[j] = temp.position[j+1] ;
						} // for
						temp.end = temp.end - 1 ;
					} // if
					else { // 不存在 -->那就是assignment 
						symtab.pop_back() ;
						StatementAssign( temp, str, code.label ) ;
						code = clear ; // 初始化 
						done = true ;
					} // else
					
				} // else if
				
			} // while
		} // TranslateOneline
		
		string FullString( Code code, int line ) {
			char number[4] ; // 第幾行-->string 
			itoa( line, number, 10 ) ;
			string num = number ;
			itoa( code.op1.type, number, 10 ) ;
			string type = number ;
			itoa( code.op1.position, number, 10 ) ;
			string position = number ;
			string temp = "" ;
			temp = num + "	((" + type + "," + position + ")	," ; 
			if ( code.opnum == 1 ) {
				temp = temp + "	,	,	)	" + code.text ;
			} // if
			else if ( code.opnum == 2 ) {
				itoa( code.op4.type, number, 10 ) ;
				type = number ;
				itoa( code.op4.position, number, 10 ) ;
				position = number ;
				temp = temp + "	,	,(" + type + "," + position + ")	)	" + code.text ;
			} // else if
			else if ( code.opnum == 3 ) {
				itoa( code.op2.type, number, 10 ) ;
				type = number ;
				itoa( code.op2.position, number, 10 ) ;
				position = number ;
				temp = temp + "(" + type + "," + position + ")	,	," ;
				itoa( code.op4.type, number, 10 ) ;
				string type = number ;
				itoa( code.op4.position, number, 10 ) ;
				string position = number ;
				temp = temp + "(" + type + "," + position + ")	)	" + code.text ;
			} // else if
			else {
				itoa( code.op2.type, number, 10 ) ;
				type = number ;
				itoa( code.op2.position, number, 10 ) ;
				position = number ;
				temp = temp + "(" + type + "," + position + ")	," ;
				itoa( code.op3.type, number, 10 ) ;
				type = number ;
				itoa( code.op3.position, number, 10 ) ;
				position = number ;
				temp = temp + "(" + type + "," + position + ")	," ;
				itoa( code.op4.type, number, 10 ) ;
				type = number ;
				itoa( code.op4.position, number, 10 ) ;
				position = number ;
				temp = temp + "(" + type + "," + position + ")	)	" + code.text ;
			} // else
			
			if ( temp[temp.length()-1] != '\n' ) {
				temp = temp + "\n" ;
			} // if
			return temp ;
		} // FullString
		
		void Translate() { // table6 table7 從1開始算唷唷-->存的是改過的位置(從1開始 
			int line = 0 ;
			int index = 0 ;
			CheckSyntax() ;
			Token t ;
			string str = "" ;
			Code code ;
			Code clear ;
			vector<Code> err ;
			int arr[100] = {-1} ;
			while ( line < source.size() ) {
				t = token.at(line) ;
				string temp = source.at(line) ; // 第line+1行的東西
				temp = temp.substr( 0, temp.length() - 1 ) ; // 不拿換行 
				if ( !t.error ) { // 文法沒錯 
					TranslateOneline( t, temp ) ;
				} // if
				else {
					arr[index] = table6.size() ;
					index++ ;
					str = t.text + "\n" ;
					code.error = true ;
					code.text = str ;
					err.push_back( code ) ;
					code = clear ;
				} // else
				line++ ;
			} // while
			
			TwoPass() ;
			int e = 0 ;
			index = 0 ;
			for ( int j = 0 ; j < table6.size() ; j++ ) {
				code = table6.at(j) ;
				if ( arr[e] == j ) {
					str = err.at(index).text ;
					WriteFile( str ) ;
					j-- ;
					arr[e] = -1 ;
					e++ ;
					index++ ;
				} // if
				else {
					str = FullString( code, j+1 ) ;
					WriteFile( str ) ;
				} // else
			} // for
			
			if ( arr[e] == table6.size() ) { // 最後一行有錯 
				str = err.at(index).text ;
				WriteFile( str ) ;
			} // if
			/*int j = 0 ;
			for ( j = 0 ; j < symtab.size() ; j++ ) {
				cout << symtab.at(j).name << endl ;
			} // for
			for ( j = 0 ; j < var.size() ; j++ ) {
				cout << var.at(j).name << endl ;
			}
			for ( j = 0 ; j < 100 ; j++ ) {
				cout << table5[j].name << ", scope:" << table5[j].scope << ", type:" << table5[j].type << ", pointer:" ;
				if ( table5[j].pointer.type == 7 ) {
					while ( !table5[j].q.empty() ) {
						cout << table5[j].pointer.type << "," << table5[j].q.front() << "  " ;
						table5[j].q.pop() ;
					} // while
					cout << endl ;
				} // if
				else {
					cout << table5[j].pointer.type << "," << table5[j].pointer.position << endl ;
				} // else
				
			}
			for ( j = 0 ; j < table7.size() ; j++ ) {
				cout << "table7 : " << j << " " << table7.at(j) << endl ;
			}*/
		} // Translate
		
		void TwoPass() {
			int i = 0 ;
			int index = 0 ;
			int j = 0 ;
			while ( i < two.size() ) {
				index = two.at(i).index ;
				j = 0 ;
				string label = two.at(i).text ;
				while ( j < table6.size() ) {
					if ( table6.at(j).label != "" ) {
						if ( label == table6.at(j).label ) {
							table6.at(index).op4.type = 6 ;
							table6.at(index).op4.position = j + 1 ;
							break ;
						} // if
					} // if
					j++ ;
				} // while
				
				i++ ;
			} // while
		} // TwoPass
		
		void WriteFile( string line ) { 
			string name = fileName + "_IntermediateCode.txt" ;
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
		
};

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


int main() {
	char command = '1' ;
	bool exit = false ;
	while ( !exit ) {
		if ( command == '1' ) {
			LexicalAnalysis lex ; 
			Syntax_Analysis syn ;
			Lexical_Analysis( lex ) ;
			syn.Copy( lex ) ;
			syn.CreateTokenList() ;
			syn.Translate() ;
		} // if
		cout << "請輸入是否繼續，繼續請輸入1，結束請輸入任意鍵\n:" ;
		cin >> command ;
		if ( command != '1' ) {
			exit = true ;
		} // else
	} // while
	
	
} // main()




