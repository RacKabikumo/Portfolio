# include <iostream>
# include <stdio.h>
# include <stdlib.h> // atoi
# include <string.h>
# include <string>
# include <vector>
# include <sstream> // 10��8,16 
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
	string name ; // �����s���� 
	string type ; // ������token 
	string op ; // proj2 operator
	int line ; // error�� �b�ĴX�� 
	int position ; // error�� ��token�Ĥ@�Ӧr�Ҧb����m 
	int len ; // name���� 
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
			temp = temp + ch ; // ����]��J
			msource.push_back( temp ) ;
			return true ;
		} // if
		else {
			if ( temp == "" ) { // �o���SŪ��F�� 
				cout << "> ERROR (no more input) : END-OF-FILE encountered" << endl ;
				mexit = true ;
				return false ;
			} // if
			else {
				temp = temp + "\n" ;
				msource.push_back( temp ) ;
				return true ; // �o����Ū��F��-->�p�G���Owhite space ���N�S�N�q
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
		if ( str == "." ) { // �u��1���I 
			return "dot" ;
		} // if
		else if ( str == "t" || str == "#t" ) { // T�����p --> true
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
				else if ( str[index] == '.' ) { // ���I!-->�����float��symbol 
					times++ ;
					if ( times <= 1 ) {
						type = "float" ;
					} // if
					else {
						return "symbol" ;
					} // else
				} // else if
				else { // ���O�p�Ƥ]���O�Ʀr 
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
					if ( str[index] == '\"' ) { // ���ݭn�e�����ϱ׽u 
						temp = temp + '\"' ;
					} // if
					else if ( str[index] == '\'' ) {
						temp = temp + "\'" ;
					} // else if
					else if ( str[index] == '\\' ) {
						temp = temp + "\\" ;
					} // else if
					else { // �n�e����\ �Y�ϬO\n��\t�]�n�d�� 
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
					return false ; // �٨S�ݨ�ĤG��"�N�S�F-->�����D! 
				} // else
			} // if
			else if ( str[index] == '\"' ) {
				temp = temp + str[index] ;
				name = temp ; // �Ǧ^�h 
				return true ; // ���ĤG��"�F 
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
		return false ; // ���S���ĤG��" 
		
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
			if ( str[index] == '(' ) { // ���A�� LP 
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
				if ( index < str.length() ) { // ���ݬO���Onil 
					if ( str[index] == ')' ) { // �Onil 
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
			else if ( str[index] == ')' ) { // �k�A�� RP 
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
			else if ( str[index] == '\"' ) { // �r��  string
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
				if ( CheckString( index, str, table.name, table.len ) ) { // �ˬd�þ�zstring�O�_��"" 
					// table.len = table.name.length() ;
					table.type = "string" ;
					mtoken.push_back( table ) ;
				} // if
				else { // �S�����"!
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
			else if ( str[index] == '\'' ) { // ��޸� QUOTE 
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
			else if ( str[index] == ';' ) { // ����
				if ( temp != "" ) {
					table.name = temp ;
					table.len = temp.length() ;
					table.position = index - temp.length() ;
					table.type = CheckType( temp ) ;
					table.op = CheckOP( temp ) ;
					mtoken.push_back( table ) ;
					temp = "" ;
				} // if
				
				index = str.length() ; // ���Ѥ��Φs �᭱�]���ΤF 
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
		int line = msource.size() ; // �C��Ū�@��
		if ( index >= mtoken.size() ) { // �����F �٨Smatch�� 
			if ( GetLine() ) { // �bŪ�@�� 
				Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken
				return Syntax_Analysis( index ) ; // �o��index�٨S�� 
			} // if
			else { // Ū���F 
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
				if ( GetLine() ) { // �bŪ�@�� 
					Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken
					return Syntax_Analysis( index )	;
				} // if
				else { // Ū���F 
					return false ;
				} // else
			} // if
			else {
				return Syntax_Analysis( index )	;
			} // else
		} // if
		else if ( mtoken.at( index ).type == "LP" ) { // ���A�� 
			index++ ;
			while ( index >= mtoken.size() ) {
				if ( GetLine() ) { // �bŪ�@�� 
					line = msource.size() - 1 ;
					Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken
				} // if
				else { // Ū���F 
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
				while ( Syntax_Analysis( index ) ) { // ���A���᪺S-exp���O�諸 �n�ݤU�@�ӬO���O�k�A�� 
					index++ ;
					while ( index >= mtoken.size() ) {
						if ( GetLine() ) { // �bŪ�@�� 
							line = msource.size() - 1 ;
							Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken
						} // if
						else { // Ū���F 
							return false ;
						} // else
					} // while
					
					if ( mtoken.at( index ).type == "RP" ) { // �k�A�� 
						return true ;
					} // else if
					else if ( mtoken.at( index ).type == "dot" ) { // ��DOT�᭱�n�Os-exp 
						index++ ;
						while ( index >= mtoken.size() ) {
							if ( GetLine() ) { // �bŪ�@�� 
								line = msource.size() - 1 ;
								Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken
							} // if
							else { // Ū���F 
								return false ;
							} // else
						} // while
						
						if ( Syntax_Analysis( index ) ) { // �p�G�Os-exp�᭱�@�w�n���k�A�� 
							index++ ;
							while ( index >= mtoken.size() ) {
								if ( GetLine() ) { // �bŪ�@�� 
									line = msource.size() - 1 ;
									Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken
								} // if
								else { // Ū���F 
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
				return false ; // ���X�ӥN���� 
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
		else { // ���Oatom �]���O�}�Y���r�� �����D! 
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
					// ���A���e�S��.�n�ۤv�[�Wnil ""���@�}�l�n�L�� LP�]�n�L 
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					BuildTree( head->left, index, size, "" ) ; // ���A���٨S�� 
					if ( head->left->type == "" ) { // �S��F�� �n�d���e����new 
						head->left = NULL ;
					} // if
					
					head->right = new Tree() ;
					head->right->left = NULL ;
					head->right->right = NULL ;
					index++ ;
					if ( index < size ) {
						BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
						if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
							head->right->name = "" ; // ���ݭn�L�X�Ӫ�nil 
							head->right->type = "nil" ;
						} // if
					} // if
					
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					BuildTree( head->left, index, size, "" ) ; // ���A���٨S�� 
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
				} // else if
				else {
					if ( lastType == "dot" ) {
						// head->type = "LP" ; // ���ΦL�X�Ӫ����A��
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
						if ( head->left->type == "" ) { // �S��F�� �n�d���e����new 
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
							if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
								head->right = NULL ;
							} // if
						} // if
							
					} // if
					
					
					if ( head->hasDot ) { // ���A����.
						if ( index + 1 < size && mtoken.at( index + 1 ).type == "RP" ) {
					 	// �n���L
							index++ ; 
						} // if
					} // if
				} // else
					
			} // if
			else if ( mtoken.at( index ).type == "int" ) {
				string temp = mtoken.at( index ).name ;
				if ( temp[0] == '+' ) {
					temp = temp.substr( 1, temp.length() - 1 ) ; // �h��+
					mtoken.at( index ).name = temp ; 
				} // if
				
				if ( lastType == "LP" || lastType == "dot" ) { // ������ 
					head->name = mtoken.at( index ).name ;
					head->number = atof( head->name.c_str() ) ;
					head->type = "int" ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // �᭱�O) 
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
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
				} // else if
				else {
					if ( head == mhead ) { // �@�}�l�N�Oatom ������ 
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
							if ( mtoken.at( index ).type == "RP" ) { // �p�G�᭱�O)�n�[nil 
								head->right->name = "" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "float" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // ������ 
					head->name = mtoken.at( index ).name ;
					head->type = "float" ;
					head->number = atof( head->name.c_str() ) ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // �᭱�O)
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
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // �@�}�l�N�Oatom ������ 
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
							if ( mtoken.at( index ).type == "RP" ) { // �p�G�᭱�O)�n�[nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "string" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // ������ 
					head->name = mtoken.at( index ).name ;
					head->type = "string" ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // �᭱�O)
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->type = "string" ;
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // �@�}�l�N�Oatom ������ 
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
							if ( mtoken.at( index ).type == "RP" ) { // �p�G�᭱�O)�n�[nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
				
			} // else if
			else if ( mtoken.at( index ).type == "nil" ) {
				if ( lastType == "dot" ) { // ���n�L 
					head->name = "nil" ;
					head->type = "nil" ;
					head->hasDot = true ;
					index++ ; // �᭱�O)
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = "nil" ;
					head->left->type = "nil" ;
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead || lastType == "LP" ) { // �@�}�l�N�Oatom ������ 
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
							if ( mtoken.at( index ).type == "RP" ) { // �p�G�᭱�O)�n�[nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
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
							if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
								head->right = NULL ;
							} // if
						} // if
							
					} // else if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "true" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // ������ 
					head->name = "#t" ;
					head->type = "true" ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // �᭱�O)
					} // if
				} // if
				else if ( lastType == "quote" ) {
					head->type = "empty" ;
					head->left = new Tree() ;
					head->left->left = NULL ;
					head->left->right = NULL ;
					head->left->name = mtoken.at( index ).name ;
					head->left->type = "dot" ;
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // �@�}�l�N�Oatom ������ 
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
							if ( mtoken.at( index ).type == "RP" ) { // �p�G�᭱�O)�n�[nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
				
			} // else if
			else if ( mtoken.at( index ).type == "symbol" ) {
				if ( lastType == "LP" || lastType == "dot" ) { // ������ 
					head->name = mtoken.at( index ).name ;
					head->type = "symbol" ;
					head->op = mtoken.at( index ).op ;
					if ( lastType == "dot" ) {
						head->hasDot = true ;
						index++ ; // �᭱�O)
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
					head->right = new Tree() ; // ������nil
					head->right->left = NULL ;
					head->right->right = NULL ;
					head->right->type = "nil" ; 
					head->right->name = "nil" ;
				} // else if
				else {
					if ( head == mhead ) { // �@�}�l�N�Oatom ������ 
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
							if ( mtoken.at( index ).type == "RP" ) { // �p�G�᭱�O)�n�[nil 
								head->right->name = "nil" ;
								head->right->type = "nil" ;
							} // if
							else {
								BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
								if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
									head->right = NULL ;
								} // if
							} // else
							
						} // if
					} // if
				} // else
			} // else if
			else if ( mtoken.at( index ).type == "quote" ) {
				if ( lastType == "LP" || lastType == "" ) { // ���γ�empty 
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
						if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
							head->right = NULL ;
						} // if
					} // if
				} // if
				else {
					head->type = "empty" ;
					head->left = new Tree() ; // �񥪬A�� 
					head->left->name = "" ;
					// head->left->type = "LP" ;
					head->left->type = "empty" ;
					head->left->left = new Tree() ;	// ��'��quote 
					head->left->left->left = NULL ;
					head->left->left->right = NULL ;
					head->left->left->name = "quote" ;
					// head->left->left->type = "quote" ;
					head->left->left->type = "symbol" ;
					head->left->left->op = "quote" ;
					head->left->right = new Tree() ; // ��quote���F��
					head->left->right->left = NULL ;
					head->left->right->right = NULL ; 
					index++ ;
					if ( index < size ) {
						BuildTree( head->left->right, index, size, mtoken.at( index - 1 ).type ) ;
						if ( head->left->right->type == "" ) { // �S��F�� �n�d���e����new 
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
							index-- ; // �h�^�h 
						} // if
						else {
							BuildTree( head->right, index, size, mtoken.at( index - 1 ).type ) ;
							if ( head->right->type == "" ) { // �S��F�� �n�d���e����new 
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
			else { // �I�N���L 
				index++ ;
				if ( index < size ) {
					BuildTree( head, index, size, mtoken.at( index - 1 ).type ) ;
				} // if
			} // else
		} // if
		
		if ( head->left != NULL && head->right == NULL ) { // ���䦳�F��k��S�� 
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

	TreePtr CheckSymbol( string name, bool & fn, DefSym & sym ) { // �w�L 
		int i = 0 ;
		DefSym s ; // ���ӥ���ϰ��ܼ� �]�����ϰ��ܼ����ӥ��� 
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
	
	TreePtr Check_Local_Symbol( string name, bool & fn, DefSym & sym ) { // �w�L 
		int i = 0 ;
		DefSym s ; // ���ӥ���ϰ��ܼ� �]�����ϰ��ܼ����ӥ��� 
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
			if ( head->type == "nil" ) { // �k�䨫�쩳 �ݭn�Onil 
				return true ;
			} // if
			
			if ( print ) {
				err_type = "non_list" ;
				error_msg = "ERROR (non-list) : " ;
				// �n�L�F��-->�^�h�A�L(�����Dhead�b��)
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
		else if ( head->left == NULL && head->type == "symbol" ) { // �u��symbol 
			bool fn = false ;
			DefSym sym ;	 
			TreePtr arg = NULL ;
			if ( local ) { // ����local
				arg = Check_Local_Symbol( head->name, fn, sym ) ; 
				if ( arg == NULL ) { // local�S���hglobal�� 
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
				if ( fn ) { // �N��Ouser�w�q��fn 
				
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
		else if ( head->left != NULL && head->left->type == "symbol" ) { // ���u��symbol 
			bool fn = false ;
			DefSym sym ;
			TreePtr arg = NULL ; 
			if ( local ) {
				arg = Check_Local_Symbol( head->left->name, fn, sym ) ;
				if ( arg == NULL ) {
					arg = CheckSymbol( head->left->name, fn, sym ) ; // �o�ӭn�ݬO���Ofn�� 
				} // if
			} // if
			else {
				arg = CheckSymbol( head->left->name, fn, sym ) ; // �o�ӭn�ݬO���Ofn�� 
			} // else
			
			temp = head->right ;
			if ( !CheckList( head, true, err_type, error_msg ) ) { // ���Olist --> �Lerror 
				out = head ;
				return false ;
			} // if
			else if ( arg == NULL && head->left->op != "" ) { // �ˬd�O���O��ͪ��B�� 
				if ( !out_ring ) { // ���O�b�~�� �b���� 
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
				
				if ( head->left->op == "define" ) { // �b�~��! 
					// �ˬd�ѼƮ榡�藍��!
					if ( temp == NULL ) {
						;
					} // if
					else if ( temp->left == NULL ) {
						;
					} // else if
					else if ( temp->left->type != "symbol" && temp->left->type != "empty" ) { // 3�i��ݭn�� 
						// ���Osymbol�]���Oempty-->�����D 
						;
					} // else if
					else if ( temp->left->type == "empty" &&
										!CheckList( temp->left, true, err_type, error_msg ) ) {
						// �p�G�Oempty�N�n�ˬd�O���Olist 
						out = temp->left ;
						return false ;
					} // else if
					else if ( temp->left->type == "empty" && temp->left->left->type != "symbol" ) {
						// �p�G�Oempty ����n�Osymbol�~�i�H��� 
						;
					} // else if
					else if ( temp->left->op != "" ) { // �O���\�઺ 
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
						// �٦���h��! 
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
					if ( temp->left->type == "symbol" ) { // ��ǩw�q�ܼƪ����� 
						temp = temp->right ; // define�u�neval�ĤG�ӰѼ� (���Olambda���n!!)
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
						if ( run->type == "empty" && run->left->op == "lambda" ) { // lambda�S�O�B�z 
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
							
							// �]�w�ѼƼƶq 
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
							temp = temp->right ; // define�u�neval�ĤG�ӰѼ� (���Olambda���n!!)
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
					else { // �w�qfn ������ ����define 
						// ���ˬd�ڪ�fn�ѼƬO���O�ܼ�(symbol)
						TreePtr run = temp->left ;
						while ( run != NULL && run->type != "nil" ) {
							if ( run->left->type != "symbol" ) {
								error_msg = "ERROR (DEFINE format) : " ;
								err_type = "format" ;
								out = head ;
								return false ;
							} // if
							else if ( run->left->op != "" ) { // ����w�q���\�઺�F�� 
								error_msg = "ERROR (DEFINE format) : " ;
								err_type = "format" ;
								out = head ;
								return false ;
							} // else if
							
							run = run->right ;
						} // while
						
						run = temp->left->right ;
						vector<string> arg_name ;
						while ( run != NULL && run->type != "nil" ) { // ���Ѽ� 
							arg_name.push_back( run->left->name ) ;
							run = run->right ;
						} // while
						
						for ( int j = 0 ; j < arg_name.size() ; j++ ) {
							for ( int k = 0 ; k < arg_name.size() ; k++ ) {
								if ( k != j ) {
									if ( arg_name.at( j ) == arg_name.at( k ) ) { // ���Ʃw�q 
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
								if ( run->left != NULL && run->left->name == "else" ) { // �̫�@�ӬOelse
									run = run->right ;
									while ( run != NULL && run->type != "nil" ) {
										correct = Eval( run->left, false, out, err_type, error_msg, local, false ) ;
										if ( !correct ) {
											if ( err_type == "return_value" ) {
												// last one����S��return value(??
												if ( run->right == NULL || run->right->type == "nil" ) {
													return false ;
												} // if	
												else { // �����Sreturn value���O�� 
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
								if ( err_type == "return_value" ) { // �o�����ӬO����S��return value 
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
												// last one����S��return value(??
												if ( run->right == NULL || run->right->type == "nil" ) {
													return false ;
												} // if	
												else { // �����Sreturn value���O�� 
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
						if ( temp->right->right->type == "nil" ) { // 2�� 
							correct = true ;
						} // if
						else if ( temp->right->right->right != NULL && 
											temp->right->right->right->type == "nil" ) { // 3�� 
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
						if ( err_type == "return_value" ) { // �o�����ӬO����S��return value 
							error_msg = "ERROR (unbound test-condition) : " ;
							err_type = "return_value_cond" ;
							out = temp_origin->left ;
						} // if
						
						return false ;
					} // if
					
					
					if ( left->type == "nil" ) { // false �n���ĤG�ӰѼ� 
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
					// �h�ˬd�۹������Ѽ� 
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
					while ( temp != NULL && temp->type != "nil" ) { // and�J��nil�N�^��nil 
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
					
					return true ; // ����out�w�g�O�̫�@�ӪF��F 
					
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
					while ( temp != NULL && temp->type != "nil" ) { // or�J��Ĥ@�Ӥ��Onil�N�^�ǤF 
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
					
					return true ; // ����out�w�g�O�̫�@�ӪF��F 
					
				} // else if
				else if ( head->left->name == "let" ) { // let >= 2 
					if ( temp != NULL && temp->right != NULL && temp->right->type != "nil" ) {
						// �u��0��1�ӰѼ�
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
							if ( label == NULL ) { // ����w�q���fn 
								error_msg = "ERROR (LET format) : " ;
								err_type = "format" ;
								out = origin ;
								return false ;
							} // if
								
						} // else if
						
						// �ݭn���i�H�w�q���F��� 
						if ( walk->right->type != "empty" ) {
							error_msg = "ERROR (LET format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // if
						else if ( walk->right->right->type != "nil" ) { // �Ӧh�F 
							error_msg = "ERROR (LET format) : " ;
							err_type = "format" ;
							out = origin ;
							return false ;
						} // else if
						
						run = run->right ;
					} // while
					
					
					run = temp->left ; // �w�q�Ѽ��o-->�s�� 
					walk = run->left ; // �ѼƦ�m 
					vector<DefSym> temp_var ;
					vector<DefSym> origin_local_var = mlocaltab ; // �Ψ��x�slet���e��local�ܼ� ����let�n�_�� 
					DefSym t ;
					t.arg = NULL ;
					t.fn = false ;
					t.op = "" ;
					t.argNum = 0 ;
					t.lambda = false ;
					while ( run != NULL && run->type != "nil" ) {
						walk = run->left ;

						// ���S���D�n�өw�qlocal�ܼ��o
						
						for ( int j = 0 ; j < temp_var.size() ; j++ ) {
							if ( temp_var.at( j ).name == t.name ) { // ���Ʃw�q! 
								error_msg = "ERROR (LET format) : " ;
								err_type = "format" ;
								out = origin ;
								return false ;
							} // if
						} // for
						
						correct = Eval( walk->right->left, false, out, err_type, error_msg, local, false ) ; 
						// eval�Ѽ� 
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
					
					run = temp->left ; // �w�q�Ѽ��o-->�s�� 
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
							if ( mlocaltab.at( k ).name == temp_var.at( j ).name ) { // �n�л\(? 
								mlocaltab.at( k ) = temp_var.at( j ) ; 
								put_done = true ;
							} // if
						} // for
						
						if ( !put_done ) {
							mlocaltab.push_back( temp_var.at( j ) ) ;
						} // if
						
					} // for
					
					
					// �ѼƩw�q���o�o �}�l����
					// mlocaltab = temp_var ; // �ϰ��ܼƥu���ڭ��w�q�����ǰ� 
					temp_var.clear() ;
					temp = temp->right ;
					while ( temp != NULL && temp->type != "nil" ) {
						correct = Eval( temp->left, false, out, err_type, error_msg, true, false ) ; 
						if ( !correct ) {
							return false ;
						} // if
						
						temp = temp->right ;
					} // while
					
					mlocaltab = origin_local_var ; // �_��쥻��table �Ȥ��ᦳ�Ψ�W�٤@�˪�symbol
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
						out = head ; // �u���return�o �^�h�A�B�z�A!
					} // else
					 
					return true ;
					
				} // else if
				else { // ��ѼƼƶq 
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
							if ( head->left->op == "quote" ) { // ���ݭnevaluate�Ѽ� 
								return Evaluate( head, out, err_type, error_msg ) ;
							} // if
							
							correct = true ;
						} // else
						
						while ( temp != NULL && temp->type != "nil" ) { // ����2�ӰѼ�
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
						
						while ( temp != NULL && temp->type != "nil" ) { // ����2�ӰѼ�
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
						
						while ( temp != NULL && temp->type != "nil" ) { // ����2�ӰѼ�
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
									// last one����S��return value(??
									if ( temp->right == NULL || temp->right->type == "nil" ) {
										return false ;
									} // if	
									else { // �����Sreturn value���O�� 
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
						while ( temp != NULL && temp->type != "nil" ) { // ����2�ӰѼ�
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
				// �n��arg���� �P�_�O���O�ڭn����-->evalate!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
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
					else if ( !correct ) { // ���� 
						return false ;
					} // else if
					else {
						error_msg = "ERROR (attempt to apply non-function) : " ;
						err_type = "non_fn" ;
						return false ;
					} // else
				} // if
				else { // �����ǰt �٭n�B�zlambda���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
					TreePtr temp = head->right ;
					TreePtr origin = NULL ;
					CopyTree( head, origin ) ;
					int argNum = 0 ;
					while ( temp != NULL && temp->type != "nil" ) { // eval�Ѽ� 
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
					
					if ( argNum != sym.argNum ) { // ����ƶq�藍�� 
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
					
					vector<DefSym> origin_var = mlocaltab ; // �����n�s�^�� 
					vector<DefSym> temp_localtab ;
					if ( argNum != 0 ) { // ���Ѽ� �n���]�w�n�Ѽ� 
						temp = head->right ;
						TreePtr run = temp->left ; // �Ĥ@�ӰѼ�
						TreePtr fn_arg = sym.arg ;
						TreePtr temp_origin = origin->right ;
						DefSym t ; // local �Ѽƥ� 
						while ( temp != NULL && temp->type != "nil" ) { // �ˬd�Ҧ��Ѽ� 
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
							if ( temp->left->op != "" ) { // ���ڪ��ѼƴN�Ofn�� 
								DefSym temp_sym ;
								run = CheckSymbol( run->name, fn, temp_sym ) ;
								if ( run == NULL ) { // �O���op 
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
							bool done = false ; // ����쥻���R���A��i�h 
							for ( int j = 0 ; j < mlocaltab.size() && !done ; j++ ) {
								if ( mlocaltab.at( j ).name == t.name ) {
									mlocaltab.erase( mlocaltab.begin() + j ) ;
									done = true ;
								} // if
							} // for
							*/
							
							// temp_localtab.push_back( t ) ; // ��eval�n���ѼƦs��local�ܼ�Tab 
							temp_localtab.push_back( t ) ; // ��eval�n���ѼƦs��local�ܼ�Tab 
							temp = temp->right ;
							temp_origin = temp_origin->right ;
							fn_arg = fn_arg->right ;
						} // while
					} // if 
					
					TreePtr run = NULL ;
					CopyTree( arg, run ) ; // ���F������fn���� 
					mlocaltab = temp_localtab ; // �{�b�ϰ��ܼƥu�����w�q���F�� 
					if ( keep_local ) { // �n�O�dlocal�ܼ� 
						// ��local��@global��!
						for ( int j = 0 ; j < temp_localtab.size() ; j++ ) { // �ڳo��fn�٨S���� 
							mlocaltab.push_back( temp_localtab.at( j ) ) ;
						} // for
						
						// ����local�s���ӳo�˧ڪ�lambda�N��ΤF�� 
					} // if
					
					temp_localtab.clear() ;
					correct = Eval( run, false, out, err_type, error_msg, true, false ) ;
					
					
					if ( keep_local && out->left != NULL && out->left->op == "lambda" ) {
						// �p�G�Olambda �{�b���Ѽƫ᭱�٭n�Ψ� 
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
			else { // �S���symbol�]���O��ͪ��B�⪺symbol 
				error_msg = "ERROR (unbound symbol) : " + head->left->name ;
				err_type = "unbound_symbol" ;
				out = NULL ;
				return false ;
			} // else
				 
		} // else if
		else if ( head->left != NULL && head->left->type == "empty" ) { // �ݥL(head->left)�O���Olist 
			// �i��Olambda �n�p���P�P gogogogo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
			if ( !CheckList( head, true, err_type, error_msg ) ) { // ���Olist 
				out = head ;
				return false ;
			} // if
			
			if ( head->left->left->type == "symbol" ) { // peek�O���Olambda 
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
				// (( lambda ..... ) .... ) ���ˬdlambda���S���� 
				if ( head->left->left->name != "lambda" ) {
					// �Ofn �ܼƭn���d�� �����A�^�_�� �g
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
					else if ( !correct ) { // ���� 
						if ( err_type == "return_value" ) {
							error_msg = "ERROR (no return value) : " ;
							err_type = "return_value_in" ; // error�N�O�S��return value 
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
					// ���n���_��-->�ڭn�w�q! 
					if ( correct ) { // lambda��k�S�� �n���� 
						// ���ݶǤJ���Ѽ� �藍�� 
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
						
						// �ݰѼƼƶq�藍��
						int formal_arg = 0 ;
						TreePtr run = head->left->right->left ; // lambda�Ѽ�
						while ( run != NULL && run->type != "nil" ) {
							formal_arg++ ;
							run = run->right ;
						} // while
						
						if ( formal_arg != actual_arg ) { // �ƶq���� 
							error_msg = "ERROR (incorrect number of arguments) : " ;
							err_type = "argNum" ;
							out = head->left->left ;
							mlocaltab = temp_local_tab ;
							return false ;
						} // if
						
						// �w�q�F���Ѽ�
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
						
						run = head->left->right->right ; // �n�����Ʊ� 
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
						
						// out�N�O�̫�@�ӭn�^�Ǫ�����
						// �n�s�^�ӭ쥻�����A 
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
			else if ( !correct ) { // ���� 
				if ( err_type == "return_value" ) {
					error_msg = "ERROR (no return value) : " ;
					err_type = "return_value_in" ; // error�N�O�S��return value 
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
			if ( !CheckList( head, true, err_type, error_msg ) ) { // ���Olist --> �Lerror 
				out = head ;
				return false ;
			} // if
			
			error_msg = "ERROR (attempt to apply non-function) : " ;
			err_type = "non_fn" ;
			out = head->left ;
			return false ;
		} // else
		
		/*
		while ( temp != NULL && temp->type != "nil" ) { // ����2�ӰѼ�
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
		Infix( out ) ; // �h���̭����S��N�q��fn 
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
		if ( head->right->left->type == "symbol" ) { // �w�q�ܼ� 
			TreePtr arg = CheckSymbol( head->right->left->name, fn, sym ) ; 
			if ( arg != NULL ) { // ��w�q�L���R�� �b�w�q�@��! 
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
		else { // �w�qfn
			TreePtr arg = CheckSymbol( head->right->left->left->name, fn, sym ) ; 
			if ( arg != NULL ) { // ��w�q�L���R�� �b�w�q�@��! 
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
		TreePtr temp = head->right->left ; // �Ѽ� 
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
		stringstream ss ; // �sname�� 
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
		stringstream ss ; // �sname�� 
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
		if ( head->right->left->type != "nil" ) { // true�n�ରflase 
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
		if ( run->type != "string" ) { // type���o 
			error_msg = "ERROR (string-append with incorrect argument type) : " ;
			err_type = "arg_type" ;
			out = run ;
			return false ;
		} // if
		
		temp = temp->right ;
		while ( temp != NULL && temp->type != "nil" ) {
			run = temp->left ;
			if ( run->type != "string" ) { // type���o 
				error_msg = "ERROR (string-append with incorrect argument type) : " ;
				err_type = "arg_type" ;
				out = run ;
				return false ;
			} // if
			
			str = str.substr( 0, str.length() - 1 ) ; // �����̫᪺"! 
			str = str + run->name.substr( 1, run->name.length() - 1 ) ; // �����Ĥ@��" !
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
		if ( left == right ) { // ���V�@�˪��a�� 
			out = new Tree() ;
			out->left = NULL ;
			out->right = NULL ;
			out->type = "true" ;
			out->name = "#t" ;
			return true ; 
		} // if
		else if ( left->left == NULL && right->left == NULL ) { // ���Oatom 
			if ( left->name == right->name && left->type != "string" && left->type == right->type ) { // �W�r�@�� 
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
				if ( CompareNode( left->left, right->left ) ) { // �S���~�~�� 
					return CompareNode( left->right, right->right ) ;
				} // if
				
				return false ;
			} // if
			else {
				return false ;
			} // else
		} // if
		else if ( left != NULL || right != NULL ) { // �u���@�ӬONULL 
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
		if ( in->left == NULL ) { // �u��atom 
			out = in ;
			return true ;
		} // if
		else if ( in->left->type != "symbol" ) { // �Oatom 
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
		
		return false ; // �p�G�����O�N�����D�� 
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
					line = msource.size() - 1 ; // �C��Ū�@�� 
					Lexical_Analysis( msource.at( line ), line + 1 ) ; // ��mtoken 	
				} // if
				else { // Ū���F 
					fin = true ;
					msource.clear() ;
					mtoken.clear() ;
				} // else
			} // if
			else {
				for ( int i = 0 ; i < distanceOfline ; i++ ) { 
					// �b�Ĥ@��token�Ҧb����ƫe���F�賣���n�F 
					msource.erase( msource.begin() ) ;
				} // for
				
				for ( int i = 0 ; i < mtoken.size() ; i++ ) { // ��s��Ƹ�column 
					mtoken.at( i ).line = mtoken.at( i ).line - distanceOfline ;
					mtoken.at( i ).position = mtoken.at( i ).position - distanceOfcolumn ; 
				} // for 
				
				
			} // else
			
			if ( !fin ) {
				if ( mtoken.size() != 0 ) { // �S��error 
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
						BuildTree( mhead, start, index, "" ) ; // �٨S�B�z����Ƹ�column�n�B�z 
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
							else { // ���� 
								cout << error_msg ;
								if ( out != NULL ) { // �ݭn�L 
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
							
							mlocaltab.clear() ; // local���M�� 
							for ( int i = 0 ; i < index - 1 ; i++ ) { // �d�@�� ����coulun 
								mtoken.erase( mtoken.begin() ) ;
							} // for
							
							distanceOfline = mtoken.at( 0 ).line - 1 ; // �stoken�}�l�����
							distanceOfcolumn = mtoken.at( 0 ).position + mtoken.at( 0 ).len ; 
							// �stoken���_�l��m 
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



