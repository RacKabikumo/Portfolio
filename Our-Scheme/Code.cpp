# include <iostream>
# include <stdio.h>
# include <stdlib.h> // atoi
# include <string.h>
# include <string>
# include <vector>
# include <sstream> // 10轉8,16 
# include <queue>
# include <stack>
using namespace std; 

/*
terminal (mtoken) :
  LEFT-PAREN 	LP	 // '('
  RIGHT-PAREN	RP	 // ')'
  INT   		int     // e.g., '123', '+123', '-123'
  STRING  		string   // "string's (example)." (strings do not extend across lines)
  DOT     		dot       // '.'
  FLOAT   		float     // '123.567', '123.', '.567', '+123.4', '-.123'
  NIL     		nil      // 'nil' or '#f', but not 'NIL' nor 'nIL'
  T       		true     // 't' or '#t', but not 'T' nor '#T'
  QUOTE   		quote   // '
  SYMBOL     	symbol   // a consecutive sequence of printable characters that are
                   // not numbers, strings, #t or nil, and do not contain 
                   // '(', ')', single-quote, double-quote, semi-colon and 
                   // white-spaces ; 
                   // Symbols are case-sensitive 
                   // (i.e., uppercase and lowercase are different);
*/

struct Table {
	string name ; // 本身叫什麼 
	string type ; // 為哪種token 
	string op ; // proj2 operator
	int line ; // error用 在第幾行 
	int position ; // error用 為token第一個字所在的位置 
	int len ; // name長度 
} ;

struct Tree {
	string name ;
	string type ;
	string op ;
	float number ;
	bool hasDot ;
	Tree* left ;
	Tree* right ;
};

typedef Tree* TreePtr ;

struct DefSym {
	TreePtr head ;
	TreePtr arg ;
	string name ;
	int argNum ;
	string op ;
	bool fn ;
	bool lambda ;
};

class OurScheme {

public :
	bool mverbose_mode ;
	vector<string> msource ;
	vector<Table> mtoken ;
	vector<DefSym> msymtab ;
	vector<DefSym> mlocaltab ;
	TreePtr mhead ;
	bool mexit ;
	
	void Init() {
		mverbose_mode = true ;
		mhead = NULL ;
		mexit = false ;
		msource.clear() ;
		mtoken.clear() ;
		msymtab.clear() ;
		mlocaltab.clear() ;
	} // Init()
	
	bool GetLine() {
		int num = 0 ;
		char ch = '\0' ;
		string temp = "" ;
		num = scanf( "%c", &ch ) ;
		while ( num != -1 && ch != '\n' ) {
			temp = temp + ch ;
			num = scanf( "%c", &ch ) ;
		} // while
		
		if ( num != -1 ) {
			temp = temp + ch ; // 換行也放入
			msource.push_back( temp ) ;
			return true ;
		} // if
		else {
			if ( temp == "" ) { // 這次沒讀到東西 
				cout << "> ERROR (no more input) : END-OF-FILE encountered" << endl ;
				mexit = true ;
				return false ;
			} // if
			else {
				temp = temp + "\n" ;
				msource.push_back( temp ) ;
				return true ; // 這次有讀到東西-->如果都是white space 那就沒意義
			} // else
		
		} // else
		
	} // GetLine()
	
	bool IsWhiteSpace( char ch ) {
		if ( ch != ' ' && ch != '\t' && ch != '\n' ) {
			return false ;
		} // if
		else {
			return true ;
		} // else
		
	} // IsWhiteSpace()
	
	string CheckType( string str ) {
		if ( str == "." ) { // 只有1個點 
			return "dot" ;
		} // if
		else if ( str == "t" || str == "#t" ) { // T的情況 --> true
			return "true" ; 
		} // else if
		else if ( str == "#f" || str == "nil" ) { // nil -- > nil 
			return "nil" ; 
		} // else if
		else { // INT FLOAT SYMBOL
			int index = 0 ;
			string type = "int" ;
			int times = 0 ;
			bool hasNum = false ;
			while ( index < str.length() ) {
				if ( str[index] >= '0' && str[index] <= '9' ) {
					hasNum = true ;
				} // if
				else if ( str[index] == '.' ) { // 有點!-->但表示float或symbol 
					times++ ;
					if ( times <= 1 ) {
						type = "float" ;
					} // if
					else {
						return "symbol" ;
					} // else
				} // else if
				else { // 不是小數也不是數字 
					if ( index == 0 && ( str[index] == '+' || str[index] == '-' ) ) {
						;
					} // if
					else {
						return "symbol" ;
					} // else
					
				} // else
				
				index++ ;
			} // while
			
			if ( hasNum ) {
				return type ;
			} // if
			else {
				return "symbol" ;
			} // else
		} // else
	} // CheckType()
	
	bool CheckString( int &index, string str, string &name, int &len ) {
		string temp = "\"" ;
		len = index - 1 ;
		while ( index < str.length() && str[index] != '\"' ) {
			if ( str[index] == '\\' ) {
				index++ ;
				if ( index < str.length() ) {
					if ( str[index] == '\"' ) { // 不需要前面的反斜線 
						temp = temp + '\"' ;
					} // if
					else if ( str[index] == '\'' ) {
						temp = temp + "\'" ;
					} // else if
					else if ( str[index] == '\\' ) {
						temp = temp + "\\" ;
					} // else if
					else { // 要前面的\ 即使是\n跟\t也要留著 
						if ( str[index] == 'n' ) {
							temp = temp + "\n" ;
						} // if
						else if ( str[index] == 't' ) {
							temp = temp + "\t" ;
						} // else if
						else {
							temp = temp + "\\" + str[index] ;
						} // else
						
					} // else
				} // if
				else {
					return false ; // 還沒看到第二個"就沒了-->有問題! 
				} // else
			} // if
			else if ( str[index] == '\"' ) {
				temp = temp + str[index] ;
				name = temp ; // 傳回去 
				return true ; // 找到第二個"了 
			} // else if
			else {
				temp = temp + str[index] ;
			} // else
			
			index++ ;
		} // while
		
		len = index - len ;
		if ( index < str.length() ) {
			temp = temp + str[index] ;
			name = temp ;
			len = name.length() ;
			return true ;
		} // if
		
		name = temp ;
		return false ; // 都沒找到第二個" 
		
	} // CheckString()
	
	// Constructors quote define cut_part boolean cal eq begin if_cond clear
	// exit
	
	string CheckOP( string temp ) {
		string t = "" ;
		if ( temp == "cons" || temp == "list" ) {
			return "Constructors" ;
		} // if
		else if ( temp == "quote" ) {
			return temp ;
		} // else if
		else if ( temp == "define" ) {
			return temp ;
		} // else if
		else if ( temp == "car" || temp == "cdr" ) {
			return "cut_part" ;
		} // else if
		else if ( temp == "atom?" || temp == "pair?" || temp == "list?" || temp == "null?" ||
							temp == "integer?" || temp == "real?" || temp == "number?" || 
							temp == "string?" || temp == "boolean?" || temp == "symbol?" ) {
			return "boolean" ;
		} // else if
		else if ( temp == "+" || temp == "-" || temp == "*" || temp == "/" ||
							temp == "not" || temp == "and" || temp == "or" || temp == ">" || temp == ">=" ||
							temp == "<" || temp == "<=" || temp == "=" || temp == "string-append" ||
							temp == "string>?" || temp == "string<?" || temp == "string=?" ) {
			return "cal" ;
		} // else if
		else if ( temp == "eqv?" || temp == "equal?" ) {
			return "eq" ;
		} // else if
		else if ( temp == "begin" ) {
			return temp ;
		} // else if
		else if ( temp == "if" || temp == "cond" ) {
			return "if_cond" ;
		} // else if
		else if ( temp == "clean-environment" ) {
			return "clear" ;
		} // else if
		else if ( temp == "let" ) {
			return "let" ;
		} // else if
		else if ( temp == "lambda" ) {
			return "lambda" ;
		} // else if
		else if ( temp == "verbose?" ) {
			return "verbose?" ;
		} // else if
		else if ( temp == "verbose" ) {
			return "verbose" ;
		} // else if
		else if ( temp == "exit" ) {
			return "exit" ;
		} // else if
		else {
			return t ;
		} // else
	} // CheckOP()
	
	void Lexical_Analysis( string str, int line ) {
		int index = 0 ;
		string temp = "" ;
		Table table ;
		table.line = line ;	
		while ( index < str.length() ) {
			if ( str[index] == '(' ) { // 左括號 LP 
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
				
				index++ ;
				if ( index < str.length() ) { // 偷看是不是nil 
					if ( str[index] == ')' ) { // 是nil 
						table.name = "nil" ;
						table.len = 2 ;
						table.position = index - 1 ;
						table.type = "nil" ;
						mtoken.push_back( table ) ;
					} // if
					else {
						index-- ;
						table.name = "(" ;
						table.position = index ;
						table.len = 1 ;
						table.type = "LP" ;
						mtoken.push_back( table ) ;
					} // else
				} // if
					
			} // if
			else if ( str[index] == ')' ) { // 右括號 RP 
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
				
				table.name = ")" ;
				table.position = index ;
				table.len = 1 ;
				table.type = "RP" ;
				mtoken.push_back( table ) ;
			} // else if
			else if ( str[index] == '\"' ) { // 字串  string
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
				
				table.position = index ;
				index++ ;
				if ( CheckString( index, str, table.name, table.len ) ) { // 檢查並整理string是否有"" 
					// table.len = table.name.length() ;
					table.type = "string" ;
					mtoken.push_back( table ) ;
				} // if
				else { // 沒有兩個"!
					table.name = "> ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
					char c = line + '0' ;
					table.name = table.name + c + " Column " ;
					string index_str = "" ;
					for ( int j = index ; j > 0 ; j = j / 10 ) {
						c = ( j % 10 ) + '0' ;
						index_str = c + index_str ;
					} // for
					
					table.name = table.name + index_str ;
					table.type = "error" ;
					mtoken.push_back( table ) ;
				} // else
				
			} // else if
			else if ( str[index] == '\'' ) { // 單引號 QUOTE 
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
				
				table.name = "\'" ;
				table.position = index ;
				table.len = 1 ;
				table.type = "quote" ;
				mtoken.push_back( table ) ;
			} // else if
			else if ( str[index] == ';' ) { // 註解
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
				
				index = str.length() ; // 註解不用存 後面也不用了 
			} // else if
			else if ( !IsWhiteSpace( str[index] ) ) {
				temp = temp + str[index] ;
			} // else if
			else {
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;			
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
			} // else
			
			index++ ;
		} // while
		
	} // Lexical_Analysis()
	
	bool Syntax_Analysis( int &index ) {
		int line = msource.size() ; // 每次讀一行
		if ( index >= mtoken.size() ) { // 不夠了 還沒match到 
			if ( GetLine() ) { // 在讀一行 
				Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken
				return Syntax_Analysis( index ) ; // 這個index還沒做 
			} // if
			else { // 讀完了 
				return false ;
			} // else
		} // if
		else if ( mtoken.at( index ).type == "error" ) {
			cout << "> ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
			cout << mtoken.at( index ).line << " Column " ;
			cout << mtoken.at( index ).position + mtoken.at( index ).len << endl << endl ;
			mtoken.clear() ;
			msource.clear() ;
			return false ;
		} // else if
		else if ( mtoken.at( index ).type == "quote" ) {
			index++ ;
			if ( index >= mtoken.size() ) {
				if ( GetLine() ) { // 在讀一行 
					Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken
					return Syntax_Analysis( index )	;
				} // if
				else { // 讀完了 
					return false ;
				} // else
			} // if
			else {
				return Syntax_Analysis( index )	;
			} // else
		} // if
		else if ( mtoken.at( index ).type == "LP" ) { // 左括號 
			index++ ;
			while ( index >= mtoken.size() ) {
				if ( GetLine() ) { // 在讀一行 
					line = msource.size() - 1 ;
					Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken
				} // if
				else { // 讀完了 
					return false ;
				} // else
			} // while
			
			if ( mtoken.at( index ).type == "RP" ) {
				mtoken.erase( mtoken.begin() + index ) ;
				index-- ;
				mtoken.at( index ).name = "nil" ;
				mtoken.at( index ).type = "nil" ;
				mtoken.at( index ).line = msource.size() ;
				return true ;
			} // if
			else { 
				while ( Syntax_Analysis( index ) ) { // 左括號後的S-exp都是對的 要看下一個是不是右括號 
					index++ ;
					while ( index >= mtoken.size() ) {
						if ( GetLine() ) { // 在讀一行 
							line = msource.size() - 1 ;
							Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken
						} // if
						else { // 讀完了 
							return false ;
						} // else
					} // while
					
					if ( mtoken.at( index ).type == "RP" ) { // 右括號 
						return true ;
					} // else if
					else if ( mtoken.at( index ).type == "dot" ) { // 有DOT後面要是s-exp 
						index++ ;
						while ( index >= mtoken.size() ) {
							if ( GetLine() ) { // 在讀一行 
								line = msource.size() - 1 ;
								Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken
							} // if
							else { // 讀完了 
								return false ;
							} // else
						} // while
						
						if ( Syntax_Analysis( index ) ) { // 如果是s-exp後面一定要接右括號 
							index++ ;
							while ( index >= mtoken.size() ) {
								if ( GetLine() ) { // 在讀一行 
									line = msource.size() - 1 ;
									Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken
								} // if
								else { // 讀完了 
									return false ;
								} // else
							} // while
							
							if ( mtoken.at( index ).type == "RP" ) {
								return true ;
							} // if
							else {
								cout << "> ERROR (unexpected token) : " ;
								cout << "')' expected when token at Line " ;
								cout << mtoken.at( index ).line ;
								cout << " Column " << mtoken.at( index ).position + 1 ;
								cout << " is >>" << mtoken.at( index ).name ;
								cout << "<<" << endl << endl ;
								mtoken.clear() ;
								msource.clear() ;
								return false ;
							} // else
						} // if
						else {
							return false ;
						} // else
					} // else if
				} // while
				
				mtoken.clear() ;
				msource.clear() ;
				return false ; // 跳出來代表有錯 
			} // else
		} // else if
		else if ( mtoken.at( index ).type == "int" || mtoken.at( index ).type == "string" ) {
			return true ;
		} // else if
		else if ( mtoken.at( index ).type == "float" || mtoken.at( index ).type == "nil" ) {
			return true ;
		} // else if
		else if ( mtoken.at( index ).type == "true" || mtoken.at( index ).type == "symbol" ) {
			return true ;
		} // else if
		else { // 不是atom 也不是開頭的字們 有問題! 
			cout << "> ERROR (unexpected token) : " ;
			cout << "atom or '(' expected when token at Line " << mtoken.at( index ).line ;
			if ( mtoken.at( index ).type == "RP" ) {
				cout << " Column " << mtoken.at( index ).position + 1 << " is >>)<<" << endl << endl ;
			} // if
			else {
				cout << " Column " << mtoken.at( index ).position + 1 << " is >>.<<" << endl << endl ;
			} // else
			
			mtoken.clear() ;
			msource.clear() ;
			return false ;
		} // else
			
	} // Syntax_Analysis()
	
	void BuildTree( TreePtr head, int &index, int size, string lastType ) { 
		if ( index < size ) {
			if ( mtoken.at( index ).type == "LP" ) { 
				if ( lastType != "dot" && lastType != "" && lastType != "LP" && lastType != "quote" ) { 
					// 左括號前沒有.要自己加上nil ""為一開始要印的 LP也要印 
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					BuildTree( head->left, index, size, "" ) ; // 左括號還沒放 
					if ( head->left->type == "" ) { // 沒放東西 要削掉前面的new 
						head->left = NULL ;
					} // if
					
					head->right = new Tree() ;
					head->right->left = NULL ;
					head->right->right = NULL ;
					index++ ;
					if ( index < size ) {
						BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
						if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
							head->right->name = "" ; // 不需要印出來的nil 
							head->right->type = "nil" ;
						} // if
					} // if
					
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					BuildTree( head->left, index, size, "" ) ; // 左括號還沒放 
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
				} // else if
				else {
					if ( lastType == "dot" ) {
						// head->type = "LP" ; // 不用印出來的左括號
						head->type = "empty" ;
						head->hasDot = true ;
						head->name = "" ; 
					} // if
					else {
						// head->type = "LP" ;
						head->type = "empty" ;
						head->name = "" ;
					} // else
					
					if ( head->left == NULL ) {
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						index++ ;
						BuildTree( head->left, index, size, mtoken.at( index - 1 ).type ) ;
						if ( head->left->type == "" ) { // 沒放東西 要削掉前面的new 
							head->left = NULL ;
						} // if
					} // if
					
					if ( index < size && head->right == NULL ) {
						head->right = new Tree() ;
						head->right->left = NULL ;
						head->right->right = NULL ;
						index++ ;
						if ( index < size ) {
							BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
							if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
								head->right = NULL ;
							} // if
						} // if
							
					} // if
					
					
					if ( head->hasDot ) { // 此括號有.
						if ( index + 1 < size && mtoken.at( index + 1 ).type == "RP" ) {
					 	// 要跳過
							index++ ; 
						} // if
					} // if
				} // else
					
			} // if
			else if ( mtoken.at( index ).type == "int" ) {
				string temp = mtoken.at( index ).name ;
				if ( temp[0] == '+' ) {
					temp = temp.substr( 1, temp.length() - 1 ) ; // 去掉+
					mtoken.at( index ).name = temp ; 
				} // if
				
				if ( lastType == "LP" || lastType == "dot" ) { // 直接放 
					head->name = mtoken.at( index ).name ;
					head->number = atof( head->name.c_str() ) ;
					head->type = "int" ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // 後面是) 
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->number = atof( head->left->name.c_str() ) ;
					head->left->type = "int" ;
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
				} // else if
				else {
					if ( head == mhead ) { // 一開始就是atom 直接放 
						head->name = mtoken.at( index ).name ;
						head->type = "int" ;
						head->number = atof( head->name.c_str() ) ;
					} // if
					else if ( head->left == NULL ) {
						head->type = "empty" ;
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						head->left->name = mtoken.at( index ).name ;
						head->left->type = "int" ;
						head->left->number = atof( head->left->name.c_str() ) ;
						index++ ;
						if ( index < size ) {
							head->right = new Tree() ;
							head->right->left = NULL ;
							head->right->right = NULL ;
							if ( mtoken.at( index ).type == "RP" ) { // 如果後面是)要加nil 
								head->right->name = "" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "float" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // 直接放 
					head->name = mtoken.at( index ).name ;
					head->type = "float" ;
					head->number = atof( head->name.c_str() ) ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // 後面是)
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->type = "float" ;
					head->left->number = atof( head->left->name.c_str() ) ;
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // 一開始就是atom 直接放 
						head->name = mtoken.at( index ).name ;
						head->type = "float" ;
						head->number = atof( head->name.c_str() ) ;
					} // if
					else if ( head->left == NULL ) {
						head->type = "empty" ;
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						head->left->name = mtoken.at( index ).name ;
						head->left->type = "float" ;
						head->left->number = atof( head->left->name.c_str() ) ;
						index++ ;
						if ( index < size ) {
							head->right = new Tree() ;
							head->right->left = NULL ;
							head->right->right = NULL ;
							if ( mtoken.at( index ).type == "RP" ) { // 如果後面是)要加nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "string" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // 直接放 
					head->name = mtoken.at( index ).name ;
					head->type = "string" ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // 後面是)
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->type = "string" ;
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // 一開始就是atom 直接放 
						head->name = mtoken.at( index ).name ;
						head->type = "string" ;
					} // if
					else if ( head->left == NULL ) {
						head->type = "empty" ;
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						head->left->name = mtoken.at( index ).name ;
						head->left->type = "string" ;
						index++ ;
						if ( index < size ) {
							head->right = new Tree() ;
							head->right->left = NULL ;
							head->right->right = NULL ;
							if ( mtoken.at( index ).type == "RP" ) { // 如果後面是)要加nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
				
			} // else if
			else if ( mtoken.at( index ).type == "nil" ) {
				if ( lastType == "dot" ) { // 不要印 
					head->name = "nil" ;
					head->type = "nil" ;
					head->hasDot = true ;
					index++ ; // 後面是)
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = "nil" ;
					head->left->type = "nil" ;
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead || lastType == "LP" ) { // 一開始就是atom 直接放 
						head->name = "nil" ;
						head->type = "nil" ;
					} // if
					else if ( head->left == NULL ) {
						head->type = "empty" ;
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						head->left->name = "nil" ;
						head->left->type = "nil" ;
						index++ ;
						if ( index < size ) {
							head->right = new Tree() ;
							head->right->left = NULL ;
							head->right->right = NULL ;
							if ( mtoken.at( index ).type == "RP" ) { // 如果後面是)要加nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
					else if ( head->right == NULL ) {
						head->right = new Tree() ;
						head->right->left = NULL ;
						head->right->right = NULL ;
						index++ ;
						if ( index < size ) {
							BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
							if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
								head->right = NULL ;
							} // if
						} // if
							
					} // else if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "true" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // 直接放 
					head->name = "#t" ;
					head->type = "true" ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // 後面是)
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->type = "dot" ;
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // 一開始就是atom 直接放 
						head->name = "#t" ;
						head->type = "true" ;
					} // if
					else if ( head->left == NULL ) {
						head->type = "empty" ;
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						head->left->name = "#t" ;
						head->left->type = "true" ;
						index++ ;
						if ( index < size ) {
							head->right = new Tree() ;
							head->right->left = NULL ;
							head->right->right = NULL ;
							if ( mtoken.at( index ).type == "RP" ) { // 如果後面是)要加nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
				
			} // else if
			else if ( mtoken.at( index ).type == "symbol" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // 直接放 
					head->name = mtoken.at( index ).name ;
					head->type = "symbol" ;
					head->op = mtoken.at( index ).op ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // 後面是)
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->op = mtoken.at( index ).op ;
					head->left->type = "symbol" ;
					head->right = new Tree() ; // 直接放nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // 一開始就是atom 直接放 
						head->name = mtoken.at( index ).name ;
						head->type = "symbol" ;
						head->op = mtoken.at( index ).op ;
					} // if
					else if ( head->left == NULL ) {
						head->type = "empty" ;
						head->left = new Tree() ;
						head->left->left = NULL ;
						head->left->right = NULL ;
						head->left->name = mtoken.at( index ).name ;
						head->left->op = mtoken.at( index ).op ;
						head->left->type = "symbol" ;
						index++ ;
						if ( index < size ) {
							head->right = new Tree() ;
							head->right->left = NULL ;
							head->right->right = NULL ;
							if ( mtoken.at( index ).type == "RP" ) { // 如果後面是)要加nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "quote" ) {
				if ( lastType == "LP" || lastType == "" ) { // 不用創empty 
					head->name = "" ;
					// head->type = "LP" ;
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->right = NULL ;
					head->left->left = NULL ;		
					head->left->op = "quote" ;	
					head->left->name = "quote" ;
					// head->left->type = "quote" ;
					head->left->type = "symbol" ;
					head->right = new Tree() ;
					head->right->left = NULL ;
					head->right->right = NULL ;
					index++ ;
					if ( index < size ) {
						BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
						if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
							head->right = NULL ;
						} // if
					} // if
				} // if
				else {
					head->type = "empty" ;
					head->left = new Tree() ; // 放左括號 
					head->left->name = "" ;
					// head->left->type = "LP" ;
					head->left->type = "empty" ;
					head->left->left = new Tree() ;	// 放'本quote 
					head->left->left->left = NULL ;
					head->left->left->right = NULL ;
					head->left->left->name = "quote" ;
					// head->left->left->type = "quote" ;
					head->left->left->type = "symbol" ;
					head->left->left->op = "quote" ;
					head->left->right = new Tree() ; // 放quote的東西
					head->left->right->left = NULL ;
					head->left->right->right = NULL ; 
					index++ ;
					if ( index < size ) {
						BuildTree( head->left->right, index, size, mtoken.at( index - 1 ).type ) ;
						if ( head->left->right->type == "" ) { // 沒放東西 要削掉前面的new 
							head->left->right = NULL ;
						} // if
					} // if

					index++ ;
					if ( index < size ) {
						head->right = new Tree() ; 
						head->right->left = NULL ;
						head->right->right = NULL ;
						if ( lastType == "quote" ) {
							head->right->type = "nil" ;
							head->right->name = "nil" ;
							index-- ; // 退回去 
						} // if
						else {
							BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
							if ( head->right->type == "" ) { // 沒放東西 要削掉前面的new 
								head->right->type = "nil" ;
							} // if
						} // else
					} // if
				} // else
					
				
			} // else if
			else if ( mtoken.at( index ).type == "RP" ) {
				head->type = "nil" ;
				head->name = "nil" ;
			} // else if
			else { // 點就跳過 
				index++ ;
				if ( index < size ) {
					BuildTree( head, index, size, mtoken.at( index - 1 ).type ) ;
				} // if
			} // else
		} // if
		
		if ( head->left != NULL && head->right == NULL ) { // 左邊有東西右邊沒有 
			head->right = new Tree() ;
			head->right->type = "nil" ;
			head->right->name = "nil" ;
			head->right->left = NULL ;
			head->right->right = NULL ;
		} // if
	} // BuildTree()
	
	bool CheckExit() {
		if ( mtoken.size() >= 3 ) {
			if ( mtoken.at( 0 ).name == "(" ) {
				if ( mtoken.at( 1 ).name == "exit" ) {
					if ( mtoken.at( 2 ).name == ")" ) {
						return true ;
					} // if
					else if ( mtoken.size() >= 5 ) {
						if ( mtoken.at( 2 ).name == "." ) {
							if ( mtoken.at( 3 ).type == "nil" ) {
								if ( mtoken.at( 4 ).name == ")" ) {
									return true ;	
								} // if
							} // if
						} // if
					} // else if
				} // if
			} // if
		} // if
					
		
		return false ;
	} // CheckExit()


	void PrettyPrint( TreePtr head, string space, bool rightChild, string &lastType, bool err ) {
		if ( head->type == "empty" ) {
			if ( lastType != "empty" ) {
				cout << space ;
			} // if
			
			/*
			if ( !err && head->left->name == "lambda" ) {
				cout << "#<procedure " << "lambda" << ">" << endl ;
			} // if
			else {
				if ( !rightChild ) {
					space = space + "  " ;
					cout << "( " ;
				} // if
				
				lastType = head->type ;
				PrettyPrint( head->left, space, false, lastType, err ) ;
				PrettyPrint( head->right, space, true, lastType, err ) ;
			} // else
			*/
			
			
			if ( !rightChild ) {
				space = space + "  " ;
				cout << "( " ;
			} // if
			
			lastType = head->type ;
			PrettyPrint( head->left, space, false, lastType, err ) ;
			PrettyPrint( head->right, space, true, lastType, err ) ;
		} // if
		else if ( head->type != "empty" ) {
			if ( rightChild && head->type == "nil" ) {
				space = space.substr( 0, space.length() - 2 ) ;
				cout << space ;
				cout << ")" << endl ;
			} // if
			else if ( rightChild ) {
				cout << space << "." << endl ;
				if ( head->type == "float" ) {
					printf( "%s%.3f\n", space.c_str(), head->number ) ; 
				} // if
				else if ( head->type == "int" ) {
					int n = atoi( head->name.c_str() ) ;
					printf( "%s%d\n", space.c_str(), n ) ;
				} // else if
				else if ( !err && head->op != "" ) {
					if ( head->op == "lambda" ) {
						cout << space << "#<procedure " << head->op << ">" << endl ;
					} // if
					else {
						cout << space << "#<procedure " << head->name << ">" << endl ;
					} // else
					
				} // else if
				else if ( head->op == "fn" || head->op == "lambda" ) {
					if ( head->op == "lambda" ) {
						cout << space << "#<procedure " << head->op << ">" << endl ;
					} // if
					else {
						cout << space << "#<procedure " << head->name << ">" << endl ;
					} // else
				} // else if
				else {
					cout << space << head->name << endl ;	
				} // else
				
				space = space.substr( 0, space.length() - 2 ) ;
				cout << space ;
				cout << ")" << endl ;
			} // else if
			else {
				if ( lastType != "empty" ) {
					cout << space ;
				} // if
				
				if ( head->type == "float" ) {
					printf( "%.3f\n", head->number ) ; 
				} // if
				else if ( head->type == "int" ) { 
					int n = atoi( head->name.c_str() ) ;
					printf( "%d\n", n ) ;
				} // else if
				else if ( !err && head->op != "" ) {
					if ( head->op == "lambda" ) {
						cout << "#<procedure " << head->op << ">" << endl ;
					} // if
					else {
						cout << "#<procedure " << head->name << ">" << endl ;
					} // else
					
				} // else if
				else if ( head->op == "fn" || head->op == "lambda" ) {
					if ( head->op == "lambda" ) {
						cout << "#<procedure " << head->op << ">" << endl ;
					} // if
					else {
						cout << "#<procedure " << head->name << ">" << endl ;
					} // else
				} // else if
				else {
					cout << head->name << endl ;
				} // else
				
			} // else
			
			lastType = head->type ;
		} // else if
	} // PrettyPrint()

	TreePtr CheckSymbol( string name, bool & fn, DefSym & sym ) { // 已印 
		int i = 0 ;
		DefSym s ; // 應該先找區域變數 因為有區域變數應該先用 
		i = msymtab.size() - 1 ;
		while ( i >= 0 ) {
			s = msymtab.at( i ) ;
			if ( s.name == name ) {
				fn = s.fn ;
				sym = s ;
				return s.head ;
			} // if
			
			i-- ;
		} // while
		
		
		return NULL ;
	} // CheckSymbol()
	
	TreePtr Check_Local_Symbol( string name, bool & fn, DefSym & sym ) { // 已印 
		int i = 0 ;
		DefSym s ; // 應該先找區域變數 因為有區域變數應該先用 
		i = mlocaltab.size() - 1 ;
		while ( i >= 0 ) {
			s = mlocaltab.at( i ) ;
			if ( s.name == name ) {
				fn = s.fn ;
				sym = s ;
				return s.head ;
			} // if
			
			i-- ;
		} // while
		
		
		return NULL ;
	} // Check_Local_Symbol()

	bool CheckList( TreePtr head, bool print, string & err_type, string & error_msg ) {
		if ( head->left == NULL && head->right == NULL ) {
			if ( head->type == "nil" ) { // 右邊走到底 需要是nil 
				return true ;
			} // if
			
			if ( print ) {
				err_type = "non_list" ;
				error_msg = "ERROR (non-list) : " ;
				// 要印東西-->回去再印(不知道head在哪)
			} // if
			
			return false ;
		} // if
		else {
			return CheckList( head->right, print, err_type, error_msg ) ;
		} // else
	} // CheckList()
	
	void CopyTree( TreePtr head, TreePtr & out ) {
		if ( head != NULL ) {
			TreePtr temp = new Tree() ;
			temp->left = NULL ;
			temp->right = NULL ;
			temp->name = head->name ;
			temp->number = head->number ;
			temp->op = head->op ;
			temp->type = head->type ;
			out = temp ;
			CopyTree( head->left, out->left ) ;
			CopyTree( head->right, out->right ) ;
		} // if
		
	} // CopyTree()
	
	
	bool Eval( TreePtr head, bool out_ring, TreePtr & out, string & err_type, 
									string & error_msg, bool local, bool keep_local ) {
		bool correct = false ;
		TreePtr temp = NULL ;
		string t = "" ;
		if ( head == NULL ) {
			out = NULL ;
			return true ;
		} // if
		else if ( head->left == NULL && head->type == "symbol" ) { // 只有symbol 
			bool fn = false ;
			DefSym sym ;	 
			TreePtr arg = NULL ;
			if ( local ) { // 先找local
				arg = Check_Local_Symbol( head->name, fn, sym ) ; 
				if ( arg == NULL ) { // local沒找到去global找 
					arg = CheckSymbol( head->name, fn, sym ) ;
				} // if
			} // if
			else {
				arg = CheckSymbol( head->name, fn, sym ) ;
			} // else
			
			if ( arg == NULL ) { 
				if ( head->op != "" ) {
					out = head ;
					return true ;
				} // if		
			} // if
			else if ( arg != NULL ) {
				if ( fn ) { // 代表是user定義的fn 
				
					head->op = sym.op ;
					/*
					if ( sym.lambda ) {
						head->op = "lambda" ;
					} // if
					else {
						head->op = "fn" ;
					} // else
					*/
					/*
					if ( sym.head->op == "lambda" ) {
						out = arg ;
					} // if
					else {
						out = head ;
					} // else
					*/
					
					out = head ;
					
					return true ;
				} // if
				
				out = arg ;
				return true ;
			} // else if
			
			if ( out_ring ) {
				out = NULL ;
			} // if
			else {
				out = head ;
			} // else
			
			error_msg = "ERROR (unbound symbol) : " + head->name ;
			err_type = "unbound_symbol" ;
			out = NULL ;
			return false ;
		} // else if
		else if ( head->left == NULL && head->type != "symbol" ) { // Atom
			out = head ;
			return true ;
		} // else if
		else if ( head->left != NULL && head->left->type == "symbol" ) { // 不只有symbol 
			bool fn = false ;
			DefSym sym ;
			TreePtr arg = NULL ; 
			if ( local ) {
				arg = Check_Local_Symbol( head->left->name, fn, sym ) ;
				if ( arg == NULL ) {
					arg = CheckSymbol( head->left->name, fn, sym ) ; // 這個要看是不是fn的 
				} // if
			} // if
			else {
				arg = CheckSymbol( head->left->name, fn, sym ) ; // 這個要看是不是fn的 
			} // else
			
			temp = head->right ;
			if ( !CheckList( head, true, err_type, error_msg ) ) { // 不是list --> 印error 
				out = head ;
				return false ;
			} // if
			else if ( arg == NULL && head->left->op != "" ) { // 檢查是不是原生的運算 
				if ( !out_ring ) { // 不是在外圍 在內圈 
					if ( head->left->op == "exit" ) {
						error_msg = "ERROR (level of EXIT)" ;
						err_type = "level" ;
					} // if
					else if ( head->left->op == "define" ) {
						error_msg = "ERROR (level of DEFINE)" ;
						err_type = "level" ;
					} // else if
					else if ( head->left->op == "clear" ) {
						error_msg = "ERROR (level of CLEAN-ENVIRONMENT)" ;
						err_type = "level" ;
					} // else if
					else {
						correct = true ;
					} // else
					
					if ( !correct ) {
						out = NULL ;
						return false ;
					} // if	
					
					correct = false ;
				} // if
				
				if ( head->left->op == "define" ) { // 在外圍! 
					// 檢查參數格式對不對!
					if ( temp == NULL ) {
						;
					} // if
					else if ( temp->left == NULL ) {
						;
					} // else if
					else if ( temp->left->type != "symbol" && temp->left->type != "empty" ) { // 3可能需要改 
						// 不是symbol也不是empty-->有問題 
						;
					} // else if
					else if ( temp->left->type == "empty" &&
										!CheckList( temp->left, true, err_type, error_msg ) ) {
						// 如果是empty就要檢查是不是list 
						out = temp->left ;
						return false ;
					} // else if
					else if ( temp->left->type == "empty" && temp->left->left->type != "symbol" ) {
						// 如果是empty 左邊要是symbol才可以唷唷 
						;
					} // else if
					else if ( temp->left->op != "" ) { // 是有功能的 
						;
					} // else if
					else if ( temp->right == NULL ) {
						;
					} // else if
					else if ( temp->right->left == NULL ) {
						;
					} // else if
					else if ( temp->right->left->type == "empty" &&
										!CheckList( temp->right->left, true, err_type, error_msg ) ) {
						out = temp->right->left ;
						return false ;
					} // else if
					else if ( temp->left->type == "symbol" && temp->right->right->type != "nil" ) { 
						// 還有更多的! 
						;
					} // else if
					else {
						correct = true ;
					} // else
		
					if ( !correct ) {
						error_msg = "ERROR (DEFINE format) : " ;
						err_type = "format" ;
						out = head ;
						return false ;
					} // if
					
					TreePtr run = temp->right->left ;
					if ( temp->left->type == "symbol" ) { // 單傳定義變數的照舊 
						temp = temp->right ; // define只要eval第二個參數 (但是lambda不要!!)
						bool lambda = false ;
						if ( temp->left->type == "empty" && temp->left->left->type == "empty" &&
								 temp->left->left->left->op == "lambda" ) {
							lambda = true ;		
						} // if
						
						correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
						if ( !correct ) {
							return false ;
						} // if
						
						if ( !lambda ) {
							temp->left = out ;
						} // if
						
						return Define( head, out, err_type, error_msg, lambda ) ;
						/* 
						if ( run->type == "empty" && run->left->op == "lambda" ) { // lambda特別處理 
							correct = Eval( run, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								return false ;
							} // if
							
							bool done = false ;
							for ( int j = 0 ; j < msymtab.size() && !done ; j++ ) {
								if ( msymtab.at( j ).name == temp->left->name ) {
									msymtab.erase( msymtab.begin() + j ) ;
									done = true ;
								} // if
							} // for
							
							// 設定參數數量 
							DefSym t ;
							t.arg = run->right->left ;
							t.lambda = true ;
							t.op = "lambda" ;
							t.fn = true ;
							t.name = temp->left->name ;
							t.argNum = 0 ;
							for ( TreePtr go = t.arg ; go->type != "nil" ; go = go->right ) {
								t.argNum++ ;
							} // for
							
							TreePtr begin = new Tree() ;
							begin->left = new Tree() ;
							begin->left->left = NULL ;
							begin->left->right = NULL ;
							begin->left->name = "begin" ;
							begin->left->op = "begin" ;
							begin->left->type = "symbol" ;
							begin->right = run->right->right ;
							t.head = begin ; 
							if ( mverbose_mode ) {
								cout << t.name << " defined" << endl ;
							} // if
							
							msymtab.push_back( t ) ;
							out = NULL ; 
							return true ;
						} // if
						else { 
							temp = temp->right ; // define只要eval第二個參數 (但是lambda不要!!)
							bool lambda = false ;
							if ( temp->left->type == "empty" && temp->left->left->type == "empty" &&
									 temp->left->left->left->op == "lambda" ) {
								lambda = true ;		
							} // if
							
							correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								return false ;
							} // if
							
							if ( !lambda ) {
								temp->left = out ;
							} // if
							
							return Define( head, out, err_type, error_msg, lambda ) ;
							
						} // else
						*/ 
					} // if
					else { // 定義fn 先不看 直接define 
						// 先檢查我的fn參數是不是變數(symbol)
						TreePtr run = temp->left ;
						while ( run != NULL && run->type != "nil" ) {
							if ( run->left->type != "symbol" ) {
								error_msg = "ERROR (DEFINE format) : " ;
								err_type = "format" ;
								out = head ;
								return false ;
							} // if
							else if ( run->left->op != "" ) { // 不能定義有功能的東西 
								error_msg = "ERROR (DEFINE format) : " ;
								err_type = "format" ;
								out = head ;
								return false ;
							} // else if
							
							run = run->right ;
						} // while
						
						run = temp->left->right ;
						vector<string> arg_name ;
						while ( run != NULL && run->type != "nil" ) { // 有參數 
							arg_name.push_back( run->left->name ) ;
							run = run->right ;
						} // while
						
						for ( int j = 0 ; j < arg_name.size() ; j++ ) {
							for ( int k = 0 ; k < arg_name.size() ; k++ ) {
								if ( k != j ) {
									if ( arg_name.at( j ) == arg_name.at( k ) ) { // 重複定義 
										error_msg = "ERROR (DEFINE format) : " ;
										err_type = "format" ;
										out = head ;
										return false ;
									} // if
								} // if
							} // for
						} // for
						
						arg_name.clear() ;
						return Define( head, out, err_type, error_msg, false ) ;
					} // else
					
					
				} // if 
				else if ( head->left->name == "cond" ) { // 1
					t = "" ;
					if ( temp->left == NULL ) {
						;
					} // if
					else if ( temp->left->type == "empty" && 
										!CheckList( temp->left, false, err_type, error_msg ) ) {
						;
					} // else if
					else if ( temp->right == NULL ) {
						;
					} // else if
					else {
						correct = true ;
					} // else
					
					if ( !correct ) {
						error_msg = "ERROR (COND format) : " ;
						err_type = "format" ;
						out = head ;
						return false ;	
					} // if
					
					TreePtr run = temp->left ;
					while ( temp != NULL && temp->type != "nil" ) {
						run = temp->left ;
						if ( run->type != "empty" ) {
							error_msg = "ERROR (COND format) : " ;
							err_type = "format" ;
							out = head ;
							return false ;
						} // if
						else if ( run->right != NULL && run->right->type != "nil" ) {
							;
						} // else if
						else {
							error_msg = "ERROR (COND format) : " ;
							err_type = "format" ;
							out = head ;
							return false ;
						} // else
						
						temp = temp->right ;
					} // while
						
					temp = head->right ;
					run = temp->left ;
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					TreePtr temp_origin = origin->right ;
					TreePtr run_origin = temp_origin->left ;
					while ( temp != NULL && temp->type != "nil" ) {
						if ( temp->left->type == "empty" && !CheckList( temp->left, false, err_type, error_msg ) ) {
							error_msg = "ERROR (COND format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // if
						else {
							if ( temp->right != NULL && temp->right->type == "nil" ) { // last one
								if ( run->left != NULL && run->left->name == "else" ) { // 最後一個是else
									run = run->right ;
									while ( run != NULL && run->type != "nil" ) {
										correct = Eval( run->left, false, out, err_type, error_msg, local, false ) ;
										if ( !correct ) {
											if ( err_type == "return_value" ) {
												// last one不能沒有return value(??
												if ( run->right == NULL || run->right->type == "nil" ) {
													return false ;
												} // if	
												else { // 中間沒return value不是錯 
													err_type = "" ;
													error_msg = "" ;
												} // else
											} // if
											else {
												return false ;
											} // else
										} // if
										else {
											run->left = out ;
										} // else
										
										run = run->right ;
									} // while
									
									return true ;
								} // if 
							} // if
							
							correct = Eval( run->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) { // 這個應該是條件沒有return value 
									error_msg = "ERROR (unbound test-condition) : " ; 
									err_type = "return_value_cond" ;
									out = run_origin->left ;
								} // if
								
								return false ;
							} // if
							else if ( out->type != "nil" ) { // true
								run->left = out ;
								run = run->right ;
								while ( run != NULL && run->type != "nil" ) {
									if ( run->left != NULL ) {
										correct = Eval( run->left, false, out, err_type, error_msg, local, false ) ;
										if ( !correct ) {
											if ( err_type == "return_value" ) {
												// last one不能沒有return value(??
												if ( run->right == NULL || run->right->type == "nil" ) {
													return false ;
												} // if	
												else { // 中間沒return value不是錯 
													err_type = "" ;
													error_msg = "" ;
												} // else
											} // if
											else {
												return false ;
											} // else
										} // if
										else {
											run->left = out ;
										} // else
									} // if
									
									run = run->right ;
								} // while
								
								return true ;
							} // else if
							
							temp = temp->right ;
							run = temp->left ;
							temp_origin = temp_origin->right ;
							run_origin = temp_origin->left ;
						} // else
						
					} // while
					
					error_msg = "ERROR (no return value) : " ;
					err_type = "return_value" ;	
					out = origin ;
					return false ;
				} // else if
				else if ( head->left->name == "if" ) {
					if ( temp != NULL && temp->right != NULL && temp->right->type != "nil" && 
							 temp->right->right != NULL ) { 
						if ( temp->right->right->type == "nil" ) { // 2個 
							correct = true ;
						} // if
						else if ( temp->right->right->right != NULL && 
											temp->right->right->right->type == "nil" ) { // 3個 
							correct = true ;		
						} // else if
						
					} // if

					if ( !correct ) {
						error_msg = "ERROR (incorrect number of arguments) : " ;
						err_type = "argNum" ;
						out = head->left ;
						return false ;	
					} // if
					
					TreePtr left = NULL ;
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					TreePtr temp_origin = origin->right ;
					correct = Eval( temp->left, false, left, err_type, error_msg, local, false ) ;
					if ( !correct ) {
						if ( err_type == "return_value" ) { // 這個應該是條件沒有return value 
							error_msg = "ERROR (unbound test-condition) : " ;
							err_type = "return_value_cond" ;
							out = temp_origin->left ;
						} // if
						
						return false ;
					} // if
					
					
					if ( left->type == "nil" ) { // false 要做第二個參數 
						if ( temp->right->right->type == "nil" ) {
							error_msg = "ERROR (no return value) : " ;
							err_type = "return_value" ;
							out = origin ;
							return false ;
						} // if
						
						temp->left = left ; 
						temp = temp->right->right ;
					} // if
					else {
						if ( temp->right->type == "nil" ) {
							error_msg = "ERROR (no return value) : " ;
							err_type = "return_value" ;
							out = origin ;
							return false ;
						} // if
						
						temp->left = left ; 
						temp = temp->right ; 
					} // else
					
					
					
					correct = Eval( temp->left, false, left, err_type, error_msg, local, false ) ; 
					// 去檢查相對應的參數 
					if ( !correct ) {
						error_msg = "ERROR (no return value) : " ;
						err_type = "return_value" ;
						out = origin ;
						return false ;
					} // if
					
					temp->left = left ; 
					out = left ;
					return true ;
						
				} // else if
				else if ( head->left->name == "and" ) {
					if ( temp != NULL && temp->right != NULL && temp->right->type != "nil" ) {
						correct = true ;
					} // if
					
					if ( !correct ) {
						error_msg = "ERROR (incorrect number of arguments) : " ;
						err_type = "argNum" ;
						out = head->left ;
						return false ;
					} // if
					
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					TreePtr temp_origin = origin->right ;
					while ( temp != NULL && temp->type != "nil" ) { // and遇到nil就回傳nil 
						correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
						if ( !correct ) {
							if ( err_type == "return_value" ) {
								error_msg = "ERROR (unbound condition) : " ;
								err_type = "return_value_and_or" ;
								out = temp_origin->left ;
							} // if
							
							return false ;
						} // if
						else if ( out->type == "nil" ) {
							return true ;
						} // else if
						
						temp = temp->right ; 
					} // while
					
					return true ; // 此時out已經是最後一個東西了 
					
				} // else if
				else if ( head->left->name == "or" ) {
					if ( temp != NULL && temp->right != NULL && temp->right->type != "nil" ) {
						correct = true ;
					} // if
					
					if ( !correct ) {
						error_msg = "ERROR (incorrect number of arguments) : " ;
						err_type = "argNum" ;
						out = head->left ;
						return false ;
					} // if
					
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					TreePtr temp_origin = origin->right ;
					while ( temp != NULL && temp->type != "nil" ) { // or遇到第一個不是nil就回傳了 
						correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
						if ( !correct ) {
							if ( err_type == "return_value" ) {
								error_msg = "ERROR (unbound condition) : " ;
								err_type = "return_value_and_or" ;
								out = temp_origin->left ;
							} // if
							
							return false ;
						} // if
						else if ( out->type != "nil" ) {
							return true ;
						} // else if
						
						temp = temp->right ; 
					} // while
					
					return true ; // 此時out已經是最後一個東西了 
					
				} // else if
				else if ( head->left->name == "let" ) { // let >= 2 
					if ( temp != NULL && temp->right != NULL && temp->right->type != "nil" ) {
						// 只有0或1個參數
						correct = true ; 
					} // if
					
					if ( !correct ) {
						error_msg = "ERROR (LET format) : " ;
						err_type = "format" ;
						out = head ;
						return false ;
					} // if
					else if ( !CheckList( temp->left, false, err_type, error_msg ) ) {
						error_msg = "ERROR (LET format) : " ;
						err_type = "format" ;
						out = head ;
						return false ;
					} // else if
					
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					TreePtr run = temp->left ;
					TreePtr walk = run->left ;
					while ( run != NULL && run->type != "nil" ) {
						walk = run->left ;
						if ( walk->type != "empty" ) {
							error_msg = "ERROR (LET format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // if
						
						
						if ( walk->left->type != "symbol" ) {
							error_msg = "ERROR (LET format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // if
						else if ( walk->left->type == "symbol" && walk->left->op != "" ) {
							bool fn = false ;
							DefSym temp_sym ;
							TreePtr label = CheckSymbol( walk->left->name, fn, temp_sym ) ;
							if ( label == NULL ) { // 不能定義原生fn 
								error_msg = "ERROR (LET format) : " ;
								err_type = "format" ;
								out = origin ;
								return false ;
							} // if
								
						} // else if
						
						// 需要有可以定義的東西啊 
						if ( walk->right->type != "empty" ) {
							error_msg = "ERROR (LET format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // if
						else if ( walk->right->right->type != "nil" ) { // 太多了 
							error_msg = "ERROR (LET format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // else if
						
						run = run->right ;
					} // while
					
					
					run = temp->left ; // 定義參數囉-->龍骨 
					walk = run->left ; // 參數位置 
					vector<DefSym> temp_var ;
					vector<DefSym> origin_local_var = mlocaltab ; // 用來儲存let之前的local變數 做完let要復原 
					DefSym t ;
					t.arg = NULL ;
					t.fn = false ;
					t.op = "" ;
					t.argNum = 0 ;
					t.lambda = false ;
					while ( run != NULL && run->type != "nil" ) {
						walk = run->left ;

						// 都沒問題要來定義local變數囉
						
						for ( int j = 0 ; j < temp_var.size() ; j++ ) {
							if ( temp_var.at( j ).name == t.name ) { // 重複定義! 
								error_msg = "ERROR (LET format) : " ;
								err_type = "format" ;
								out = origin ;
								return false ;
							} // if
						} // for
						
						correct = Eval( walk->right->left, false, out, err_type, error_msg, local, false ) ; 
						// eval參數 
						if ( !correct ) {
							if ( err_type == "return_value" ) {
								error_msg = "ERROR (unbound parameter) : " ;
								err_type = "return_value_arg" ;
							} // if
							
							return false ;
						} // if
						
						walk->right->left = out ;
						run = run->right ;
					} // while
					
					run = temp->left ; // 定義參數囉-->龍骨 
					while ( run != NULL && run->type != "nil" ) {
						walk = run->left ;
						t.name = walk->left->name ; 
						t.head = walk->right->left ;
						temp_var.push_back( t ) ;
						run = run->right ;
					} // while
					
					bool put_done = false ;
					for ( int j = 0 ; j < temp_var.size() ; j++ ) {
						put_done = false ;
						for ( int k = 0 ; k < mlocaltab.size() ; k++ ) {
							if ( mlocaltab.at( k ).name == temp_var.at( j ).name ) { // 要覆蓋(? 
								mlocaltab.at( k ) = temp_var.at( j ) ; 
								put_done = true ;
							} // if
						} // for
						
						if ( !put_done ) {
							mlocaltab.push_back( temp_var.at( j ) ) ;
						} // if
						
					} // for
					
					
					// 參數定義完囉囉 開始做事
					// mlocaltab = temp_var ; // 區域變數只有我剛剛定義的那些啦 
					temp_var.clear() ;
					temp = temp->right ;
					while ( temp != NULL && temp->type != "nil" ) {
						correct = Eval( temp->left, false, out, err_type, error_msg, true, false ) ; 
						if ( !correct ) {
							return false ;
						} // if
						
						temp = temp->right ;
					} // while
					
					mlocaltab = origin_local_var ; // 復原原本的table 怕之後有用到名稱一樣的symbol
					return correct ; 
				} // else if
				else if ( head->left->op == "lambda" ) { // lambda >= 2
					if ( temp == NULL || temp->right == NULL || temp->right->type == "nil" ) {
						;
					} // if
					else if ( temp->left == NULL || ( temp->left->type != "empty" && temp->left->type != "nil" ) ) {
						;
					} // else if
					else if ( temp->left->type == "empty" &&
										!CheckList( temp->left, false, err_type, error_msg ) ) {
						;
					} // else if
					else if ( temp->left->type == "empty" && temp->left->left->type != "symbol" ) {
						;
					} // else if
					else {
						correct = true ;
					} // else
					
					if ( !correct ) {
						error_msg = "ERROR (LAMBDA format) : " ;
						err_type = "format" ;
						out = head ;
						return false ;
					} // if
					
					if ( out_ring ) {
						out = head->left ;
					} // if
					else {
						out = head ; // 只能先return囉 回去再處理你!
					} // else
					 
					return true ;
					
				} // else if
				else { // 算參數數量 
					if ( head->left->op == "clear" || head->left->op == "exit" || head->left->op == "verbose?" ) { // 0
						if ( head->right->type != "nil" ) {
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left ;
							return false ;
						} // if
						else {
							correct = true ;
						} // else
					} // if
					else if ( head->left->op == "quote" || head->left->op == "cut_part" || 
										head->left->op == "boolean" || head->left->name == "not" ||
										head->left->op == "verbose" ) { // 1
						if ( temp->left == NULL ) {
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left ;
							return false ;
						} // if
						else if ( temp->right->type != "nil" ) {
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left ;
							return false ;
						} // else if
						else {
							if ( head->left->op == "quote" ) { // 不需要evaluate參數 
								return Evaluate( head, out, err_type, error_msg ) ;
							} // if
							
							correct = true ;
						} // else
						
						while ( temp != NULL && temp->type != "nil" ) { // 有第2個參數
							correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									error_msg = "ERROR (unbound parameter) : " ;
									err_type = "return_value_arg" ;
								} // if
				
								return false ;
							} // if
							else {
								if ( head->left->op == "cut_part" ) {
									if ( out->type != "empty" ) {
										error_msg = "ERROR (" + head->left->name + " with incorrect argument type) : " ;
										err_type = "arg_type" ;
										return false ;
									} // if
								} // if
								
								temp->left = out ;
							} // else
							
							temp = temp->right ; 
						} // while
						
					} // else if
					else if ( head->left->name == "cons" || head->left->op == "eq" ) { // 2
						if ( temp->left == NULL ) {
							;
						} // if
						else if ( temp->right == NULL ) {
							;
						} // else if
						else if ( temp->right->right == NULL ) {
							;
						} // else if
						else if ( temp->right->right->type != "nil" ) {
							;
						} // else if
						else {
							correct = true ;
						} // else
						
						if ( !correct ) {
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left ;
							return false ;
						} // if
						
						while ( temp != NULL && temp->type != "nil" ) { // 有第2個參數
							correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									error_msg = "ERROR (unbound parameter) : " ;
									err_type = "return_value_arg" ;
								} // if
				
								return false ;
							} // if
							else {
								temp->left = out ;
							} // else
							
							temp = temp->right ; 
						} // while
						
					} // else if
					else if ( head->left->op == "cal" ) { // >= 2
						if ( temp != NULL && temp->right != NULL && temp->right->type != "nil" ) {
							correct = true ;
						} // if
						
						if ( !correct ) {
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left ;
							return false ;
						} // if
						
						while ( temp != NULL && temp->type != "nil" ) { // 有第2個參數
							correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									error_msg = "ERROR (unbound parameter) : " ;
									err_type = "return_value_arg" ;
								} // if
				
								return false ;
							} // if
							else {
								if ( head->left->name == "string-append" || head->left->name == "string>?" ||
										 head->left->name == "string<?" || head->left->name == "string=?" ) {
									if ( out->type != "string" ) {
										error_msg = "ERROR (" + head->left->name + " with incorrect argument type) : " ;
										err_type = "arg_type" ;
										return false ;
									} // if
										
								} // if
								else if ( out->type != "int" && out->type != "float" ) {
									error_msg = "ERROR (" + head->left->name + " with incorrect argument type) : " ;
									err_type = "arg_type" ;
									return false ;
								} // else if
								
								temp->left = out ;
							} // else
							
							temp = temp->right ; 
						} // while
					} // else if
					else if ( head->left->op == "begin" ) { // >= 1
						if ( temp != NULL && temp->right != NULL ) {
							correct = true ;
						} // if
						
						if ( !correct ) {
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left ;
							return false ;
						} // if
						
						TreePtr run = temp->left ;
						while ( temp != NULL && temp->type != "nil" ) {
							run = temp->left ;
							correct = Eval( run, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									// last one不能沒有return value(??
									if ( temp->right == NULL || temp->right->type == "nil" ) {
										return false ;
									} // if	
									else { // 中間沒return value不是錯 
										err_type = "" ;
										error_msg = "" ;
									} // else
								} // if
								else {
									return false ;
								} // else
							} // if
							else {
								temp->left = out ;
							} // else
							
							temp = temp->right ;
						} // while
						
						return Evaluate( head, out, err_type, error_msg ) ;
					} // else if
					else {
						correct = true ;
						while ( temp != NULL && temp->type != "nil" ) { // 有第2個參數
							correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									error_msg = "ERROR (unbound parameter) : " ;
									err_type = "return_value_arg" ;
								} // if

								return false ;
							} // if
							else {
								temp->left = out ;
							} // else
							
							temp = temp->right ; 
						} // while
					} // else
				} // else
			} // else if
			else if ( arg != NULL ) {
				// 要看arg的值 判斷是不是我要的值-->evalate!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
				if ( !fn ) {
					correct = Eval( head->left, false, out, err_type, error_msg, local, false ) ;
					if ( correct && out->type == "symbol" && out->op != "" ) { 
						head->left = out ;
						return Eval( head, true, out, err_type, error_msg, local, false ) ;
					} // if
					else if ( correct && out->type == "empty" && out->left->op == "lambda" ) {
						CopyTree( out, head->left ) ;
						return Eval( head, true, out, err_type, error_msg, local, false ) ;
					} // else if
					else if ( !correct ) { // 有錯 
						return false ;
					} // else if
					else {
						error_msg = "ERROR (attempt to apply non-function) : " ;
						err_type = "non_fn" ;
						return false ;
					} // else
				} // if
				else { // 直接匹配 還要處理lambda唷唷!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
					TreePtr temp = head->right ;
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					int argNum = 0 ;
					while ( temp != NULL && temp->type != "nil" ) { // eval參數 
						argNum++ ;
						if ( temp->left->type == "symbol" ) {
							bool find = false ;
							for ( int j = 0 ; j < msymtab.size() && !find ; j++ ) {
								if ( msymtab.at( j ).name == temp->left->name ) {
									find = true ;
								} // if
							} // for
							
							if ( !find ) {
								for ( int j = 0 ; j < mlocaltab.size() && !find ; j++ ) {
									if ( mlocaltab.at( j ).name == temp->left->name ) {
										find = true ;
									} // if
								} // for
							} // if
							
							/*
							if ( !find && temp->left->op == "" ) {
								error_msg = "ERROR (unbound symbol) : " + temp->left->name ;
								err_type = "unbound_symbol" ;
								out = NULL ;
								return false ;
							} // if
							*/
						} // if
							
						
						temp = temp->right ;
					} // while
					
					if ( argNum != sym.argNum ) { // 先算數量對不對 
						if ( sym.lambda ) {
							error_msg = "ERROR (incorrect number of arguments) : lambda" ;
						} // if
						else {
							error_msg = "ERROR (incorrect number of arguments) : " + head->left->name  ;
						} // else
						
						err_type = "argNum" ;
						out = NULL ;
						return false ;
					} // if
					
					vector<DefSym> origin_var = mlocaltab ; // 做完要存回來 
					vector<DefSym> temp_localtab ;
					if ( argNum != 0 ) { // 有參數 要先設定好參數 
						temp = head->right ;
						TreePtr run = temp->left ; // 第一個參數
						TreePtr fn_arg = sym.arg ;
						TreePtr temp_origin = origin->right ;
						DefSym t ; // local 參數用 
						while ( temp != NULL && temp->type != "nil" ) { // 檢查所有參數 
							run = temp->left ;
							correct = Eval( run, false, out, err_type, error_msg, local, false ) ;
							// actual parameter 
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									error_msg = "ERROR (unbound parameter) : " ;
									err_type = "return_value_arg" ;
									out = temp_origin->left ;
								} // if
								
								return false ;
							} // if
							
							temp->left = out ;
							temp = temp->right ;
							temp_origin = temp_origin->right ;
						} // while
						
						temp = head->right ;
						temp_origin = origin->right ;
						while ( temp != NULL && temp->type != "nil" ) { 
							if ( temp->left->op != "" ) { // 那我的參數就是fn阿 
								DefSym temp_sym ;
								run = CheckSymbol( run->name, fn, temp_sym ) ;
								if ( run == NULL ) { // 是原生op 
									t.lambda = false ;
									t.argNum = 0 ;
									t.arg = NULL ;
									t.fn = false ;
									t.op = "" ;
									CopyTree( temp->left, t.head ) ;
								} // if
								else {
									CopyTree( temp_sym.arg, t.arg ) ;
									CopyTree( temp_sym.head, t.head ) ;
									t.argNum = temp_sym.argNum ;
									t.fn = temp_sym.fn ;
									t.lambda = temp_sym.lambda ;
									t.op = temp_sym.op ;
								} // else
									
							} // if
							else {
								t.lambda = false ;
								t.argNum = 0 ;
								t.arg = NULL ;
								t.fn = false ;
								t.op = "" ;
								CopyTree( temp->left, t.head ) ; 
								// t.head = out ;
							} // else
							
							t.name = fn_arg->left->name ;
							/*
							bool done = false ; // 先把原本的刪掉再放進去 
							for ( int j = 0 ; j < mlocaltab.size() && !done ; j++ ) {
								if ( mlocaltab.at( j ).name == t.name ) {
									mlocaltab.erase( mlocaltab.begin() + j ) ;
									done = true ;
								} // if
							} // for
							*/
							
							// temp_localtab.push_back( t ) ; // 把eval好的參數存到local變數Tab 
							temp_localtab.push_back( t ) ; // 把eval好的參數存到local變數Tab 
							temp = temp->right ;
							temp_origin = temp_origin->right ;
							fn_arg = fn_arg->right ;
						} // while
					} // if 
					
					TreePtr run = NULL ;
					CopyTree( arg, run ) ; // 為了不改變fn的值 
					mlocaltab = temp_localtab ; // 現在區域變數只有剛剛定義的東西 
					if ( keep_local ) { // 要保留local變數 
						// 把local當作global啦!
						for ( int j = 0 ; j < temp_localtab.size() ; j++ ) { // 我這個fn還沒做完 
							mlocaltab.push_back( temp_localtab.at( j ) ) ;
						} // for
						
						// 先把local存到近來這樣我的lambda就能用了唷 
					} // if
					
					temp_localtab.clear() ;
					correct = Eval( run, false, out, err_type, error_msg, true, false ) ;
					
					
					if ( keep_local && out->left != NULL && out->left->op == "lambda" ) {
						// 如果是lambda 現在的參數後面還要用到 
						;
					} // if
					else {
						mlocaltab = origin_var ;
					} // else
					
					 
					if ( !correct ) {
						if ( err_type == "return_value" ) {
							out = origin ;
						} // if
					} // if
					
					return correct ;

				} // else
			} // else if
			else { // 沒找到symbol也不是原生的運算的symbol 
				error_msg = "ERROR (unbound symbol) : " + head->left->name ;
				err_type = "unbound_symbol" ;
				out = NULL ;
				return false ;
			} // else
				 
		} // else if
		else if ( head->left != NULL && head->left->type == "empty" ) { // 看他(head->left)是不是list 
			// 可能是lambda 要計算嚕嚕 gogogogo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
			if ( !CheckList( head, true, err_type, error_msg ) ) { // 不是list 
				out = head ;
				return false ;
			} // if
			
			if ( head->left->left->type == "symbol" ) { // peek是不是lambda 
				TreePtr test = NULL ;
				bool temp_fn = false ;
				DefSym temp_symbol ;
				if ( local ) {
					test = Check_Local_Symbol( head->left->left->name, temp_fn, temp_symbol ) ;
					if ( test != NULL ) {
						if ( temp_symbol.lambda ) {
							head->left->left->op = "lambda" ;
						} // if
					} // if
					else {
						test = CheckSymbol( head->left->left->name, temp_fn, temp_symbol ) ;
						if ( test != NULL ) {
							if ( temp_symbol.lambda ) {
								head->left->left->op = "lambda" ;
							} // if
						} // if
					} // else
				} // if
				else {
					test = CheckSymbol( head->left->left->name, temp_fn, temp_symbol ) ;
					if ( test != NULL ) {
						if ( temp_symbol.lambda ) {
							head->left->left->op = "lambda" ;
						} // if
					} // if
				} // else
			} // if
			
			if ( head->left->left->op == "lambda" ) { 
				vector<DefSym> temp_local_tab = mlocaltab ;
				// (( lambda ..... ) .... ) 先檢查lambda有沒有錯 
				if ( head->left->left->name != "lambda" ) {
					// 是fn 變數要先留著 做完再回復啦 讚
					correct = Eval( head->left, false, out, err_type, error_msg, local, true ) ;
					if ( correct && out->op != "" ) { 
						head->left = out ;
						correct = Eval( head, true, out, err_type, error_msg, local, false ) ;
						mlocaltab = temp_local_tab ; 
						return correct ;
					} // if
					else if ( correct && out->type == "empty" && out->left->op == "lambda" ) { // lambda
						head->left = out ;
						correct = Eval( head, true, out, err_type, error_msg, local, false ) ;
						mlocaltab = temp_local_tab ; 
						return correct ;
					} // else if
					else if ( !correct ) { // 有錯 
						if ( err_type == "return_value" ) {
							error_msg = "ERROR (no return value) : " ;
							err_type = "return_value_in" ; // error就是沒有return value 
						} // if
						
						mlocaltab = temp_local_tab ; 
						return false ;
					} // else if
					else {
						error_msg = "ERROR (attempt to apply non-function) : " ;
						err_type = "non_fn" ; 
						mlocaltab = temp_local_tab ; 
						return false ;
					} // else
				} // if
				else {
					correct = Eval( head->left, false, out, err_type, error_msg, local, false ) ;
					// 不要接起來-->我要定義! 
					if ( correct ) { // lambda文法沒錯 要執行 
						// 先看傳入的參數 對不對 
						int actual_arg = 0 ;
						TreePtr temp = head->right ;
						while ( temp != NULL && temp->type != "nil" ) {
							actual_arg++ ;
							correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
							if ( !correct ) {
								mlocaltab = temp_local_tab ;
								return false ;
							} // if
							
							temp->left = out ; 
							temp = temp->right ;
						} // while
						
						// 看參數數量對不對
						int formal_arg = 0 ;
						TreePtr run = head->left->right->left ; // lambda參數
						while ( run != NULL && run->type != "nil" ) {
							formal_arg++ ;
							run = run->right ;
						} // while
						
						if ( formal_arg != actual_arg ) { // 數量不對 
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left->left ;
							mlocaltab = temp_local_tab ;
							return false ;
						} // if
						
						// 定義東西跟參數
						DefSym t ;
						temp = head->right ;
						run = head->left->right->left ;
						while ( run != NULL && run->type != "nil" ) {
							bool done = false ;
							for ( int j = 0 ; j < mlocaltab.size() && !done ; j++ ) {
								if ( mlocaltab.at( j ).name == run->left->name ) {
									mlocaltab.erase( mlocaltab.begin() + j ) ;
									done = true ;
								} // if
							} // for
							
							t.name = run->left->name ;
							t.fn = false ;
							t.op = "" ;
							t.lambda = false ;
							t.arg = NULL ;
							t.argNum = 0 ;
							t.head = temp->left ;
							
							mlocaltab.push_back( t ) ;
							temp = temp->right ;
							run = run->right ;
						} // while
						
						run = head->left->right->right ; // 要做的事情 
						TreePtr origin = NULL ;
						CopyTree( head, origin ) ;
						TreePtr run_origin = origin->left->right->right ; 
						while ( run != NULL && run->type != "nil" ) {
							correct = Eval( run->left, false, out, err_type, error_msg, true, false ) ;
							if ( !correct ) {
								if ( err_type == "return_value" ) {
									if ( run->right == NULL || run->right->type == "nil" ) {
										out = run_origin ;
										mlocaltab = temp_local_tab ;
										return false ;
									} // if
								} // if
								else {
									mlocaltab = temp_local_tab ;
									return false ;
								} // else
								
							} // if
							
							run->left = out ;
							run = run->right ;
							run_origin = run_origin->right ;
						} // while
						
						// out就是最後一個要回傳的答案
						// 要存回來原本的狀態 
						mlocaltab = temp_local_tab ;
						if ( out->type != "symbol" && out->type != "empty" ) {
							return true ;
						} // if
						
						return true ; 
					} // if
					else {
						/*
						if ( err_type == "return_value_arg" ) {
							error_msg = "ERROR (no return value) : " ;
							err_type = "return_value" ;
						} // if
						*/
						
						mlocaltab = temp_local_tab ;
						return false ;
					} // else
				} // else	
			} // if
			else {
				correct = Eval( head->left, false, out, err_type, error_msg, local, false ) ;
			} // else
			
			
			if ( correct && out->op != "" ) { 
				head->left = out ;
				correct = Eval( head, true, out, err_type, error_msg, local, false ) ;
				return correct ;
			} // if
			else if ( correct && out->type == "empty" && out->left->op == "lambda" ) { // lambda
				head->left = out ;
				correct = Eval( head, true, out, err_type, error_msg, local, false ) ;
				return correct ;
			} // else if
			else if ( !correct ) { // 有錯 
				if ( err_type == "return_value" ) {
					error_msg = "ERROR (no return value) : " ;
					err_type = "return_value_in" ; // error就是沒有return value 
				} // if
				
				return false ;
			} // else if
			else {
				error_msg = "ERROR (attempt to apply non-function) : " ;
				err_type = "non_fn" ; 
				return false ;
			} // else
		} // else if
		else { // non fn
			if ( !CheckList( head, true, err_type, error_msg ) ) { // 不是list --> 印error 
				out = head ;
				return false ;
			} // if
			
			error_msg = "ERROR (attempt to apply non-function) : " ;
			err_type = "non_fn" ;
			out = head->left ;
			return false ;
		} // else
		
		/*
		while ( temp != NULL && temp->type != "nil" ) { // 有第2個參數
			correct = Eval( temp->left, false, out, err_type, error_msg, local, false ) ;
			if ( !correct ) {
				if ( err_type == "return_value" ) {
					error_msg = "ERROR (unbound parameter) : " ;
					err_type = "return_value_arg" ;
				} // if

				return false ;
			} // if
			else {
				temp->left = out ;
			} // else
			
			temp = temp->right ; 
		} // while
		*/
		
		return Evaluate( head, out, err_type, error_msg ) ;
		
	} // Eval()


	bool Cons( TreePtr head, TreePtr & out ) {
		TreePtr temp = new Tree() ;
		temp->type = "empty" ;
		temp->left = head->right->left ;
		temp->right = head->right->right->left ;
		out = temp ;
		return true ;
	} // Cons()

	bool List( TreePtr head, TreePtr & out ) {
		out = head->right ;
		return true ;
	} // List()
	
	bool Quote( TreePtr head, TreePtr & out ) {
		out = head->right->left ;
		Infix( out ) ; // 去掉裡面有特殊意義的fn 
		return true ;
	} // Quote()
	
	void Infix( TreePtr head ) {
		if ( head != NULL ) {
			if ( head->type == "symbol" && head->op != "" ) {
				head->type = "quote_symbol" ;
				head->op = "" ;
			} // if
			
			Infix( head->left ) ;
			Infix( head->right ) ;
		} // if
	} // Infix()
	
	bool Define( TreePtr head, TreePtr & out, string & err_type, string & error_msg, bool lambda ) {
		if ( head->right->left->type == "symbol" && head->right->left->op != "" ) {
			error_msg = "ERROR (DEFINE format) : " ;
			err_type = "format" ;
			out = head ;
			return false ;
		} // if
		
		bool fn = false ;
		DefSym sym ;
		if ( head->right->left->type == "symbol" ) { // 定義變數 
			TreePtr arg = CheckSymbol( head->right->left->name, fn, sym ) ; 
			if ( arg != NULL ) { // 把定義過的刪掉 在定義一次! 
				bool exit = false ;
				for ( int j = 0 ; j < msymtab.size() ; j++ ) {
					if ( msymtab.at( j ).name == head->right->left->name ) {
						msymtab.erase( msymtab.begin() + j ) ;
						exit = true ;
					} // if
				} // for
			} // if
			
			DefSym t ;
			t.name = head->right->left->name ;
			t.arg = NULL ;
			t.lambda = false ;
			t.fn = false ;
			t.op = "" ;
			t.argNum = 0 ;
			t.head = head->right->right->left ;
			msymtab.push_back( t ) ;
			if ( mverbose_mode ) {
				cout << t.name << " defined" << endl ;
			} // if
			
			out = NULL ;
			return true ;
		} // if
		else { // 定義fn
			TreePtr arg = CheckSymbol( head->right->left->left->name, fn, sym ) ; 
			if ( arg != NULL ) { // 把定義過的刪掉 在定義一次! 
				bool exit = false ;
				for ( int j = 0 ; j < msymtab.size() ; j++ ) {
					if ( msymtab.at( j ).name == head->right->left->left->name ) {
						msymtab.erase( msymtab.begin() + j ) ;
						exit = true ;
					} // if
				} // for
			} // if
			
			DefSym t ;
			t.fn = true ;
			t.op = "fn" ;
			t.lambda = false ;
			TreePtr begin = new Tree() ;
			begin->type = "empty" ;
			begin->left = new Tree() ;
			begin->left->left = NULL ;
			begin->left->right = NULL ;
			begin->left->name = "begin" ;
			begin->left->type = "symbol" ;
			begin->left->op = "begin" ;
			begin->right = head->right->right ;
			t.head = begin ;
			
			if ( head->right->right->left != NULL && head->right->right->left->op == "lambda" ) {
				t.lambda = true ;
				t.op = "lambda" ;
				// t.head = t.head->left ;
			} // if
			else if ( head->right->right->left != NULL && head->right->right->left->left != NULL &&
								head->right->right->left->left->op == "lambda" ) {
				t.lambda = true ;	
				t.op = "lambda" ;	
			} // else if
			
			t.name = head->right->left->left->name ;
			t.arg = NULL ;
			t.argNum = 0 ;
			TreePtr temp = head->right->left->right ;
			arg = temp->left ;
			if ( arg != NULL ) {
				t.arg = temp ;
				while ( temp != NULL && temp->type != "nil" ) {
					t.argNum = t.argNum + 1 ;
					temp = temp->right ;
				} // while
			} // if
			

			msymtab.push_back( t ) ;
			if ( mverbose_mode ) {
				cout << t.name << " defined" << endl ;
			} // if
			
			out = NULL ;
			return true ;	
			
		} // else
			
			
	} // Define()
	
	bool Car( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		if ( head->right->left->type == "empty" ) {
			out = head->right->left->left ;
			return true ;
		} // if
		
		error_msg = "ERROR (car with incorrect argument type) : " ;
		err_type = "arg_type" ;
		out = head->right->left ;
		return false ;
	} // Car()
	
	bool Cdr( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		if ( head->right->left->type == "empty" ) {
			out = head->right->left->right ;
			return true ;
		} // if
		
		error_msg = "ERROR (cdr with incorrect argument type) : " ;
		err_type = "arg_type" ;
		out = head->right->left ;
		return false ;
		
	} // Cdr()

	bool Boolean( TreePtr head, TreePtr & out ) {
		TreePtr temp = head->right->left ; // 參數 
		TreePtr ans = new Tree() ;
		ans->left = NULL ;
		ans->right = NULL ;
		if ( head->left->name == "atom?" ) {
			if ( temp->left == NULL ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // if
		else if ( head->left->name == "pair?" ) { 
			if ( temp->type == "empty" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "list?" ) { 
			string err_type = "" ;
			string error_msg = "" ;
			if ( CheckList( head->right->left, false, err_type, error_msg ) ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "null?" ) { 
			if ( temp == NULL ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else if ( temp->type == "nil" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // else if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "integer?" ) { 
			if ( temp->type == "int" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "real?" ) { 
			if ( temp->type == "int" || temp->type == "float" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "number?" ) { 
			if ( temp->type == "int" || temp->type == "float" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "string?" ) { 
			if ( temp->type == "string" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "boolean?" ) { 
			if ( temp->type == "true" || temp->type == "nil" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		else if ( head->left->name == "symbol?" ) { 
			if ( temp->type == "symbol" || temp->type == "quote_symbol" ) {
				ans->type = "true" ;
				ans->name = "#t" ;
			} // if
			else {
				ans->type = "nil" ;
				ans->name = "nil" ;
			} // else
		} // else if
		
		out = ans ;
		return true ;
	} // Boolean()

	bool Add( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ; 
		TreePtr ans = new Tree() ;
		ans->left = NULL ;
		ans->right = NULL ;
		float sum = 0 ;
		float num = 0 ;
		stringstream ss ;
		bool isfloat = false ;
		while ( temp != NULL && temp->type != "nil" ) {
			if ( temp->left->type == "float" ) {
				isfloat = true ;
				num = temp->left->number ;
				sum = sum + num ;
			} // if
			else if ( temp->left->type == "int" ) {
				num = temp->left->number ;
				sum = sum + num ;
			} // else if
			else {
				error_msg = "ERROR (+ with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = temp->left ;
				return false ;
			} // else
			
			temp = temp->right ;
		} // while
		
		string str_number = "" ;
		bool exit = false ;
		ans->number = sum ;
		ss << sum ;
		ss >> str_number ;
		// ss.clear() ;
		
		if ( isfloat ) {
			ans->name = str_number ;
			ans->type = "float" ;
		} // if
		else {
			ans->name = "" ;
			for ( int j = 0 ; j < str_number.length() && !exit ; j++ ) {
				if ( str_number[j] == '.' ) {
					exit = true ;
				} // if
				
				ans->name = ans->name + str_number[j] ;
			} // for
			
			int n = atoi( ans->name.c_str() ) ;
			ans->number = n ;
			ans->type = "int" ;
		} // else
		
		out = ans ;
		return true ;
	} // Add()

	bool Sub( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ; 
		TreePtr ans = new Tree() ;
		ans->left = NULL ;
		ans->right = NULL ;
		float sum = temp->left->number ;
		bool isfloat = false ;
		if ( temp->left->type == "float" ) {
			isfloat = true ;
		} // if
		
		stringstream ss ;
		temp = temp->right ;
		float num = 0 ;
		
		while ( temp != NULL && temp->type != "nil" ) {
			if ( temp->left->type == "float" ) {
				isfloat = true ;
				num = temp->left->number ;
				sum = sum - num ;
			} // if
			else if ( temp->left->type == "int" ) {
				num = temp->left->number ;
				sum = sum - num ;
			} // else if
			else {
				error_msg = "ERROR (- with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = temp->left ;
				return false ;
			} // else
			
			temp = temp->right ;
		} // while
		
		string str_number = "" ;
		bool exit = false ;
		ans->number = sum ;
		ss << sum ;
		ss >> str_number ;
		// ss.clear() ;
		
		if ( isfloat ) {
			ans->name = str_number ;
			ans->type = "float" ;
		} // if
		else {
			ans->name = "" ;
			for ( int j = 0 ; j < str_number.length() && !exit ; j++ ) {
				if ( str_number[j] == '.' ) {
					exit = true ;
				} // if
				
				ans->name = ans->name + str_number[j] ;
			} // for
			
			int n = atoi( ans->name.c_str() ) ;
			ans->number = n ;
			ans->type = "int" ;
		} // else
		
		out = ans ;
		return true ;
	} // Sub()

	bool Mul( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ; 
		TreePtr ans = new Tree() ;
		ans->left = NULL ;
		ans->right = NULL ;
		float sum = 1 ;
		float num = 0 ;
		stringstream ss ; // 存name用 
		bool isfloat = false ;
		while ( temp != NULL && temp->type != "nil" ) {
			if ( temp->left->type == "float" ) {
				isfloat = true ;
				num = temp->left->number ;
				sum = sum * num ;
			} // if
			else if ( temp->left->type == "int" ) {
				num = temp->left->number ;
				sum = sum * num ;
			} // else if
			else {
				error_msg = "ERROR (* with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = temp->left ;
				return false ;
			} // else
			
			temp = temp->right ;
		} // while
		
		string str_number = "" ;
		bool exit = false ;
		ans->number = sum ;
		ss << sum ;
		ss >> str_number ;
		// ss.clear() ;
		
		if ( isfloat ) {
			ans->name = str_number ;
			ans->type = "float" ;
		} // if
		else {
			ans->name = "" ;
			for ( int j = 0 ; j < str_number.length() && !exit ; j++ ) {
				if ( str_number[j] == '.' ) {
					exit = true ;
				} // if
				
				ans->name = ans->name + str_number[j] ;
			} // for
			
			int n = atoi( ans->name.c_str() ) ;
			ans->number = n ;
			ans->type = "int" ;
		} // else
		
		out = ans ;
		return true ;
	} // Mul()

	bool Div( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ; 
		TreePtr ans = new Tree() ;
		ans->left = NULL ;
		ans->right = NULL ;
		float sum = temp->left->number ;
		stringstream ss ; // 存name用 
		float num = 0 ;
		bool isfloat = false ;
		if ( temp->left->type == "float" ) {
			isfloat = true ;
		} // if
		
		temp = temp->right ;
		while ( temp != NULL && temp->type != "nil" ) {
			if ( temp->left->type == "float" ) {
				isfloat = true ;
				num = temp->left->number ;
				if ( temp->left->name == "0" ) { // num == 0
					error_msg = "ERROR (division by zero) : /" ;
					err_type = "zero" ;
					out = NULL ;
					return false ;
				} // if
				
				sum = sum / num ;
			} // if
			else if ( temp->left->type == "int" ) {
				num = temp->left->number ;
				if ( num == 0 ) {
					error_msg = "ERROR (division by zero) : /" ;
					err_type = "zero" ;
					out = NULL ;
					return false ;
				} // if
				
				sum = sum / num ;
			} // else if
			else {
				error_msg = "ERROR (/ with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = temp->left ;
				return false ;
			} // else
			
			temp = temp->right ;
		} // while
		
		string str_number = "" ;
		bool exit = false ;
		ans->number = sum ;
		ss << sum ;
		ss >> str_number ;
		// ss.clear() ;
		
		if ( isfloat ) {
			ans->name = str_number ;
			ans->type = "float" ;
		} // if
		else {
			ans->name = "" ;
			for ( int j = 0 ; j < str_number.length() && !exit ; j++ ) {
				if ( str_number[j] == '.' ) {
					exit = true ;
				} // if
				
				ans->name = ans->name + str_number[j] ;
			} // for
			
			int n = atoi( ans->name.c_str() ) ;
			ans->number = n ;
			ans->type = "int" ;
		} // else
		
		out = ans ;
		return true ;
	} // Div()

	bool Not( TreePtr head, TreePtr & out ) {
		TreePtr ans = new Tree() ;
		ans->left = NULL ;
		ans->right = NULL ;
		if ( head->right->left->type != "nil" ) { // true要轉為flase 
			ans->type = "nil" ;
			ans->name = "nil" ;
		} // if
		else {
			ans->type = "true" ;
			ans->name = "#t" ;
		} // else
		
		out = ans ;
		return true ;
	} // Not()

	bool BiggerThan( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && ( run->left->type == "int" || run->left->type == "float" ) ) {
				;
			} // if
			else {
				error_msg = "ERROR (> with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->number <= right->number ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
	} // BiggerThan()

	bool BiggerAndEqual( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && ( run->left->type == "int" || run->left->type == "float" ) ) {
				;
			} // if
			else {
				error_msg = "ERROR (>= with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->number < right->number ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
	} // BiggerAndEqual()


	bool LessThan( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && ( run->left->type == "int" || run->left->type == "float" ) ) {
				;
			} // if
			else {
				error_msg = "ERROR (< with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->number >= right->number ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
	} // LessThan()
	
	bool LessAndEqual( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && ( run->left->type == "int" || run->left->type == "float" ) ) {
				;
			} // if
			else {
				error_msg = "ERROR (<= with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->number > right->number ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
	} // LessAndEqual()
	
	bool NumberEqual( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && ( run->left->type == "int" || run->left->type == "float" ) ) {
				;
			} // if
			else {
				error_msg = "ERROR (= with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->number != right->number ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
	} // NumberEqual()
	
	bool StringAppend( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr run = temp->left ;
		string str = run->name ;
		if ( run->type != "string" ) { // type錯囉 
			error_msg = "ERROR (string-append with incorrect argument type) : " ;
			err_type = "arg_type" ;
			out = run ;
			return false ;
		} // if
		
		temp = temp->right ;
		while ( temp != NULL && temp->type != "nil" ) {
			run = temp->left ;
			if ( run->type != "string" ) { // type錯囉 
				error_msg = "ERROR (string-append with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run ;
				return false ;
			} // if
			
			str = str.substr( 0, str.length() - 1 ) ; // 不拿最後的"! 
			str = str + run->name.substr( 1, run->name.length() - 1 ) ; // 不拿第一個" !
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "string" ;
		out->name = str ;
		return true ;
	} // StringAppend()
	
	bool StringBiggerThan( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && run->left->type == "string" ) {
				;
			} // if
			else {
				error_msg = "ERROR (string>? with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->name <= right->name ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
		
	} // StringBiggerThan()
	
	bool StringLessThan( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && run->left->type == "string" ) {
				;
			} // if
			else {
				error_msg = "ERROR (string<? with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->name >= right->name ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
		
	} // StringLessThan()
	
	bool StringEqual( TreePtr head, TreePtr & out, string & err_type, string & error_msg ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		TreePtr run = temp ;
		while ( run != NULL && run->type != "nil" ) {
			if ( run->left != NULL && run->left->type == "string" ) {
				;
			} // if
			else {
				error_msg = "ERROR (string=? with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run->left ;
				return false ;
			} // else
			
			run = run->right ;
		} // while
		
		temp = temp->right ;
		TreePtr right = NULL ;
		while ( temp != NULL && temp->type != "nil" ) {
			right = temp->left ;
			if ( left->name != right->name ) { // false
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // if
			
			left = right ;
			temp = temp->right ;
		} // while
		
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		out->type = "true" ;
		out->name = "#t" ;
		return true ;
		
	} // StringEqual()
	
	bool Eqv( TreePtr head, TreePtr & out ) {
		TreePtr temp = head->right ;
		TreePtr left = temp->left ;
		temp = temp->right ;
		TreePtr right = temp->left ;
		if ( left == right ) { // 指向一樣的地方 
			out = new Tree() ;
			out->left = NULL ;
			out->right = NULL ;
			out->type = "true" ;
			out->name = "#t" ;
			return true ; 
		} // if
		else if ( left->left == NULL && right->left == NULL ) { // 都是atom 
			if ( left->name == right->name && left->type != "string" && left->type == right->type ) { // 名字一樣 
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "true" ;
				out->name = "#t" ;
				return true ;
			} // if
			else {
				out = new Tree() ;
				out->left = NULL ;
				out->right = NULL ;
				out->type = "nil" ;
				out->name = "nil" ;
				return true ;
			} // else
		} // else if
		else {
			out = new Tree() ;
			out->left = NULL ;
			out->right = NULL ;
			out->type = "nil" ;
			out->name = "nil" ;
			return true ;
		} // else
	} // Eqv()
	
	bool Equal( TreePtr head, TreePtr & out ) {
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		if ( CompareNode( head->right->left, head->right->right->left ) ) {
			out->type = "true" ;
			out->name = "#t" ;
			return true ;
		} // if
		else {
			out->type = "nil" ;
			out->name = "nil" ;
			return true ;
		} // else
	} // Equal()
	
	bool CompareNode( TreePtr left, TreePtr right ) {
		if ( left != NULL && right != NULL ) {
			if ( left->type == right->type && left->name == right->name ) {
				if ( CompareNode( left->left, right->left ) ) { // 沒錯才繼續 
					return CompareNode( left->right, right->right ) ;
				} // if
				
				return false ;
			} // if
			else {
				return false ;
			} // else
		} // if
		else if ( left != NULL || right != NULL ) { // 只有一個是NULL 
			return false ;
		} // else if
		else {
			return true ;
		} // else
	} // CompareNode()
	
	bool Begin( TreePtr head, TreePtr & out ) {
		while ( head != NULL && head->right != NULL && head->right->type != "nil" ) {
			head = head->right ;
		} // while
		
		out = head->left ;
		return true ;
	} // Begin()
	
	bool Verbose( TreePtr head, TreePtr & out ) {
		TreePtr temp = head->right->left ;
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		if ( temp->type == "nil" ) {
			out->type = "nil" ;
			out->name = "nil" ;
			mverbose_mode = false ;
		} // if
		else {
			out->type = "true" ;
			out->name = "#t" ;
			mverbose_mode = true ;
		} // else
		
		return true ;
	} // Verbose()
	
	bool Is_Verbose( TreePtr head, TreePtr & out ) {
		out = new Tree() ;
		out->left = NULL ;
		out->right = NULL ;
		if ( !mverbose_mode ) {
			out->type = "nil" ;
			out->name = "nil" ;
		} // if
		else {
			out->type = "true" ;
			out->name = "#t" ;
		} // else
		
		return true ;
	} // Is_Verbose()
	
	bool Evaluate( TreePtr in, TreePtr & out, string & err_type, string & error_msg ) {
		if ( in->left == NULL ) { // 只有atom 
			out = in ;
			return true ;
		} // if
		else if ( in->left->type != "symbol" ) { // 是atom 
			out = in->left ;
			return true ;
		} // else if
		else {
			string str = in->left->name ;
			if ( str == "cons" ) {
				return Cons( in, out ) ;
			} // if
			else if ( str == "list" ) {
				return List( in, out ) ;
			} // else if
			else if ( str == "quote" ) {
				return Quote( in, out ) ;
			} // else if
			else if ( str == "define" ) {
				return Define( in, out, err_type, error_msg, false ) ;
			} // else if
			else if ( str == "car" ) {
				return Car( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "cdr" ) {
				return Cdr( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "atom?" || str == "pair?" || str == "list?" || str == "null?" ||
								str == "integer?" || str == "real?" || str == "number?" || 
								str == "string?" || str == "boolean?" || str == "symbol?" ) {
				return Boolean( in, out ) ;
			} // else if
			else if ( str == "+" ) {
				return Add( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "-" ) {
				return Sub( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "*" ) {
				return Mul( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "/" ) {
				return Div( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "not" ) {
				return Not( in, out ) ;
			} // else if
			else if ( str == ">" ) {
				return BiggerThan( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == ">=" ) {
				return BiggerAndEqual( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "<" ) {
				return LessThan( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "<=" ) {
				return LessAndEqual( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "=" ) {
				return NumberEqual( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "string-append" ) {
				return StringAppend( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "string>?" ) {
				return StringBiggerThan( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "string<?" ) {
				return StringLessThan( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "string=?" ) {
				return StringEqual( in, out, err_type, error_msg ) ;
			} // else if
			else if ( str == "eqv?" ) {
				return Eqv( in, out ) ;
			} // else if
			else if ( str == "equal?" ) {
				return Equal( in, out ) ;
			} // else if
			else if ( str == "begin" ) {
				return Begin( in, out ) ;
			} // else if
			else if ( str == "clean-environment" ) {
				msymtab.clear() ;
				if ( mverbose_mode ) {
					cout << "environment cleaned" << endl ;
				} // if
				
				out = NULL ;
				return true ;
			} // else if
			else if ( str == "verbose" ) {
				return Verbose( in, out ) ;
			} // else if
			else if ( str == "verbose?" ) {
				return Is_Verbose( in, out ) ;
			} // else if
		} // else
		
		return false ; // 如果都不是就有問題啊 
	} // Evaluate()

	// Constructors quote define cut_part boolean cal eq begin if_cond clear
	// exit

	void PrintInput() {
		int line = 0 ;
		int index = 0 ;
		int start = 0 ;
		bool fin = false ;
		string temp = "" ;
		int distanceOfcolumn = 0 ;
		int distanceOfline = 0 ;
		while ( !fin ) {
			if ( mtoken.size() == 0 ) {
				if ( GetLine() ) { 
					line = msource.size() - 1 ; // 每次讀一行 
					Lexical_Analysis( msource.at( line ), line + 1 ) ; // 切mtoken 	
				} // if
				else { // 讀光了 
					fin = true ;
					msource.clear() ;
					mtoken.clear() ;
				} // else
			} // if
			else {
				for ( int i = 0 ; i < distanceOfline ; i++ ) { 
					// 在第一個token所在的行數前的東西都不要了 
					msource.erase( msource.begin() ) ;
				} // for
				
				for ( int i = 0 ; i < mtoken.size() ; i++ ) { // 更新行數跟column 
					mtoken.at( i ).line = mtoken.at( i ).line - distanceOfline ;
					mtoken.at( i ).position = mtoken.at( i ).position - distanceOfcolumn ; 
				} // for 
				
				
			} // else
			
			if ( !fin ) {
				if ( mtoken.size() != 0 ) { // 沒有error 
					// Debug() ;
					if ( Syntax_Analysis( index ) ) {
						index++ ;
						if ( mhead == NULL ) {
							mhead = new Tree() ;
							mhead->left = NULL ;
							mhead->right = NULL ;
						} // if
						
						start = 0 ;
						mhead = new Tree() ;
						mhead->left = NULL ;
						mhead->right = NULL ;
						BuildTree( mhead, start, index, "" ) ; // 還沒處理的行數跟column要處理 
						temp = "" ;
						if ( CheckExit() ) {
							fin = true ;
							cout << "> " << endl ;
						} // if
						else {
							cout << "> " ;
							TreePtr out = NULL ;
							string error_msg = "" ;
							string err_type = "" ;
							if ( Eval( mhead, true, out, err_type, error_msg, false, false ) ) {
								if ( out != NULL ) {
									if ( out->left != NULL && out->left->name == "lambda" ) {
										out = out->left ;
									} // if
									
									PrettyPrint( out, "", false, temp, false ) ;
								} // if
							} // if
							else { // 有錯 
								cout << error_msg ;
								if ( out != NULL ) { // 需要印 
									if ( err_type != "arg_type" ) {
										Infix( out ) ;
									} // if
									
									if ( out->left != NULL && out->left->name == "lambda" ) {
										if ( err_type != "format" ) {
											out = out->left ;
										} // if
										
									} // if
									
									PrettyPrint( out, "", false, temp, true ) ;
								} // if
								else {
									cout << endl ;
								} // else
							} // else
							
							mlocaltab.clear() ; // local的清空 
							for ( int i = 0 ; i < index - 1 ; i++ ) { // 留一個 算行數coulun 
								mtoken.erase( mtoken.begin() ) ;
							} // for
							
							distanceOfline = mtoken.at( 0 ).line - 1 ; // 新token開始的行數
							distanceOfcolumn = mtoken.at( 0 ).position + mtoken.at( 0 ).len ; 
							// 新token的起始位置 
							mtoken.erase( mtoken.begin() ) ;
							if ( mtoken.size() == 0 ) {
								msource.clear() ;
							} // if
							
							cout << endl ;
							index = 0 ;	
						} // else
						
					} // if
					else {
						msource.clear() ;
						mtoken.clear() ;
						index = 0 ;
					} // else
					
				} // if
			} // if
				
			if ( mexit ) {
				fin = true ;
			} // if
		} // while
		
	} // PrintInput()
	
	void Debug() {
		int index = 0 ;
		while ( index < mtoken.size() ) {
			cout << "name : " << mtoken.at( index ).name << endl ;
			cout << "type : " << mtoken.at( index ).type << endl ;
			index++ ;
		} // while
	} // Debug()
} ;


int main() {
	printf( "Welcome to OurScheme!\n\n" ) ;
	int testNum = 0 ;
	string line = "" ;
	OurScheme scheme ;
	scheme.Init() ;
	char c = '\0' ;
	scanf( "%d%c", &testNum, &c ) ;
	scheme.PrintInput() ; // Project1
	cout << "Thanks for using OurScheme!" ;
} // main()



