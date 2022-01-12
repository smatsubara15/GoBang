#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <chrono> 
#include <ctime>
#include <cmath>
#include <iomanip>
#include <array>
#include <algorithm>
#include <utility>
#include <cstring>
#include <tuple>
#include <climits>

using namespace std; 

//struct use to hold sequences of spaces on the board (length 5 or 6 for our use). numPosPieces is the number
//of spaces occupied by the AI in that sequence. numBlanks is the number of unoccupied spaces in the sequence. 
//numNegPieces is the number of spaces occupied by the opposing player. 
struct substring {     
    string sequence;
    int numBlanks;
    int numNegPieces;
    int numPosPieces; 
};

//creates empty substring
void initializeSubstring(substring &s){
    s.sequence="";
    s.numBlanks=0;
    s.numNegPieces=0;
    s.numPosPieces=0;
}

//adds a new space to the substring and updates its values. 
void updateSubstring(substring &s, string space, string AI){
    if (space=="_"){
        s.numBlanks++;
        s.sequence+="_";
    }
    else if (space==AI){
        s.numPosPieces++;
        s.sequence+="A";
    }
    else{
        s.numNegPieces++;
        s.sequence+="P";
    }
}

void printBoard(vector<vector<string> > &board){
    string letters[]={"a","b","c","d","e","f","g","h","i","j","k","l",
        "m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    cout<<"   ";
    for(int i=0;i<board.size();i++){
        cout<<"  "<<letters[i]<<" ";
    }
    cout<<" "<<endl;
    cout<<"   ";
    for(int i=0;i<board.size();i++){
        cout<<"+---";
    }
    cout<<"+"<<endl;

    for (int i = 0; i < board.size(); i++) { 
        if(i+1<10){
            cout<<" "<<i+1<<" |";
        }
        else{
            cout<<i+1<<" |";
        }
        for (int j = 0; j < board[i].size(); j++) {
            if(board[i][j]=="_"){
                cout<<"   |";
            }
            else{
                cout << " "<<board[i][j] << " |"; 
            }
        }
        cout<<endl; 
        cout<<"   ";
        for(int j=0;j<board.size();j++){
            cout<<"+---";
        }
        cout<<"+"<<endl;
    }
    cout<<endl;
}

void parseMove(string move, int &letter, int &position, map<string,int> letter_to_int){
    string l(1,tolower(move[0]));
    letter=letter_to_int[l];
    string pos;
    for(int i=1;i<move.size();i++){
        pos=pos+move[i];
    }
    position=stoi(pos);
}

void findMyChoices(int letter, int position, unordered_set<string> &choices, vector<string> int_to_letter, vector<string> occupied_spaces,int size,string move){
    if(choices.count(move)){
        choices.erase(move);
    }
    vector<string> new_choices;
    if(position==1 && letter==0){
        new_choices.push_back(int_to_letter[letter+1]+to_string(position));
        new_choices.push_back(int_to_letter[letter]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position+1));
    }
    else if(position==1 && letter==(size-1)){
        new_choices.push_back(int_to_letter[letter]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position));
    }
    else if(position==size && letter==0){
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));   
        new_choices.push_back(int_to_letter[letter+1]+to_string(position));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position-1));
    }
    else if(position==size && letter==(size-1)){
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));  
        new_choices.push_back(int_to_letter[letter-1]+to_string(position)); 
        new_choices.push_back(int_to_letter[letter-1]+to_string(position-1));
    }
    else if(position==1){
        new_choices.push_back(int_to_letter[letter+1]+to_string(position));
        new_choices.push_back(int_to_letter[letter]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position));
    }
    else if(letter==0){
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));   
        new_choices.push_back(int_to_letter[letter+1]+to_string(position));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position-1));
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));   
        new_choices.push_back(int_to_letter[letter+1]+to_string(position-1));
    }
    else if(position==size){
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));   
        new_choices.push_back(int_to_letter[letter+1]+to_string(position));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position-1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position)); 
        new_choices.push_back(int_to_letter[letter-1]+to_string(position-1));
    }
    else if(letter==(size-1)){
        new_choices.push_back(int_to_letter[letter]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position));
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));   
        new_choices.push_back(int_to_letter[letter-1]+to_string(position-1));
    }
    else{
        new_choices.push_back(int_to_letter[letter+1]+to_string(position));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position));
        new_choices.push_back(int_to_letter[letter]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter]+to_string(position-1));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position+1));
        new_choices.push_back(int_to_letter[letter+1]+to_string(position-1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position-1));
        new_choices.push_back(int_to_letter[letter-1]+to_string(position+1));
    }
    for(int i=0;i<new_choices.size();i++){
        if(find(occupied_spaces.begin(), occupied_spaces.end(), new_choices[i]) == occupied_spaces.end()) {
            choices.insert(new_choices[i]);
        }
    }
}

int calculateScore(vector<vector<string> > board, int letter, int position, int &score, string P1,int &won){
    string AI;
    vector<substring> substrings;
    if(P1=="X"){
        AI="O";
    }
    else{
        AI="X";
    }
    int leftBound = ((letter-5)>=0) ? (letter-5):0;
    int rightBound = ((letter+5)<board.size()) ? (letter+5):board.size()-1;
    int upperBound = ((position-5)>=0) ? (position-5):0;
    int lowerBound = ((position+5)<board.size()) ? (position+5):board.size()-1;
    for(int i=leftBound;i<=rightBound-5;i++){
        int upDiagPos=position+(letter-i);
        int downDiagPos=position-(letter-i);
        substring HorizontalString;
        initializeSubstring(HorizontalString);
        if(downDiagPos>=0 && downDiagPos+5<board.size()){
            substring downDiagString;
            initializeSubstring(downDiagString);
            for(int j=0;j<6;j++){
                string downDiagSpace=board[downDiagPos+j][i+j];
                updateSubstring(downDiagString,downDiagSpace,AI);
            }
            substrings.push_back(downDiagString);
        }
        if(upDiagPos<board.size() && upDiagPos-5>=0){
            substring upDiagString;
            initializeSubstring(upDiagString);
            for(int j=0;j<6;j++){
                string upDiagSpace=board[upDiagPos-j][i+j];
                updateSubstring(upDiagString,upDiagSpace,AI);
            }
            substrings.push_back(upDiagString);
        }

        for(int j=0;j<6;j++){
            string horizontalSpace=board[position][i+j];
            updateSubstring(HorizontalString,horizontalSpace,AI);
        }
        substrings.push_back(HorizontalString);
    }
    //must include this to prevent opponent from winning with exactly 5 in a row in one of the corner spots
    substring c1;
    substring c2;
    substring c3;
    substring c4;
    initializeSubstring(c1);
    initializeSubstring(c2);
    initializeSubstring(c3);
    initializeSubstring(c4);
    for(int i=0;i<5;i++){
        string space1=board[4-i][0+i];
        string space2=board[(board.size()-5)+i][0+i];
        string space3=board[4-i][(board.size()-1)-i];
        string space4=board[(board.size()-5)+i][(board.size()-1)-i];
        updateSubstring(c1,space1,AI);
        updateSubstring(c2,space2,AI);
        updateSubstring(c3,space3,AI);
        updateSubstring(c4,space4,AI);
    }

    substrings.push_back(c1);
    substrings.push_back(c2);
    substrings.push_back(c3);
    substrings.push_back(c4);

    for(int i=upperBound;i<=lowerBound-5;i++){        
        substring downString;
        initializeSubstring(downString);
        for(int j=0;j<6;j++){
            string space=board[i+j][letter];
            updateSubstring(downString,space,AI);
        }
        substrings.push_back(downString);
    }
    for(int i=0;i<substrings.size();i++){
        string s=substrings[i].sequence;
        int blanks=substrings[i].numBlanks;
        int positives=substrings[i].numPosPieces;
        int negatives=substrings[i].numNegPieces;

        //we want to avoid losing by the opponent getting 5 in a row and we want
        //to avoid them gettting an open four because that also means they win
        if(s=="_PPPPP" || s =="PPPPP_" || s=="APPPPP" || s =="PPPPPA" || s=="PPPPP"){
            score-=100000000;
            won=1;
        }

        else if(s=="_AAAAA" || s =="AAAAA_" || s=="PAAAAA" || s =="AAAAAP" || s=="AAAAA"){
            score+=100000000;
            won=1;
        }

        //These are lines where the opponent has the possibility of getting 6 in a row. 
        //Sequence of 6 spaces where 5 of the spaces are occupied by one color, but are not in a row.
        else if(negatives==5 && blanks==1){
            score-=2000;
        }

        else if(positives==5 && blanks==1){
            score+=2000;
        }
        
        else if(s=="_PPPP_"){
            score-=100000;
        }
        else if(s=="_AAAA_"){
            score+=100000;
        }

        //one sided fours (line of 6 spaces where one color occupies either edge of the line
        //and the other color occupies the 4/5 remaining spots)
        else if(negatives==4 && (s[0]=='A' ^ s[5]=='A') && positives==1){
            score-=2000;
        }
        else if(positives==4 && (s[0]=='P' ^ s[5]=='P') && negatives==1){
            score+=2000;
        }

        else if(s=="PAAAAP"){
            score-=100;
        }
        else if(s=="APPPPA"){
            score+=100;
        }

        //open 3's (line of 6 spaces where one color occupies 3/4 middle spots and
        //there are no pieces on either side of the 4)
        else if(negatives==3 && positives==0 && s[0]=='_' && s[5]=='_'){
            score-=500;
        }
        else if(positives==3 && negatives==0 && s[0]=='_' && s[5]=='_'){
            score+=500;
        }

        //closed or one sided 3's.  (line of 6 spaces, one color on either edge, 
        //another color occupies 3/5 of the rest of the spots)
        else if(negatives==3 && positives==1 && (s[0]=='A' ^ s[5]=='A')){
            score-=250;
        }
        else if(positives==3 && negatives==1 && (s[0]=='P' ^ s[5]=='P')){
            score+=250;
        }

        //open 2's (line of 6 spaces where one color occupies 2/4 middle spots  and
        //there are no pieces on either side of the 4)
        else if(negatives==2 && positives==0 && s[0]=='_' && s[5]=='_'){
            score-=50;
        }
        else if(positives==2 && negatives==0 && s[0]=='_' && s[5]=='_'){
            score+=50;
        }

        //one sided 2's (line of 6 spaces, one color on either edge, 
        //another color occupies 2/5 of the rest of the spots)
        else if(negatives==2 && positives==1 && (s[0]=='A' ^ s[5]=='A')){
            score-=25;
        }
        else if(positives==2 && negatives==1 && (s[0]=='P' ^ s[5]=='P')){
            score +=25;
        }
    }
    return score;
}

int findMinMax(unordered_set<string> choices,vector<vector<string> > board,vector<string> int_to_letter,map<string,int> letter_to_int,string playerColor,int boardScore,int alpha){
    int min_val=INT_MAX;
    unordered_set<string>::iterator it;
    for (it = choices.begin(); it != choices.end(); ++it) {
        int letter,position,val,won;
        parseMove(*it,letter,position,letter_to_int);
        board[position-1][letter]=playerColor;
        val=calculateScore(board,letter,position-1,boardScore,playerColor,won);
        board[position-1][letter]="_";
        if(val<alpha){
            return val; 
        }
        if(val<min_val){
            min_val=val;
        }
    }
    return min_val;
}

int main(int argv, char** argc){
    char n[]="-n";
	char l[]="-l";
    int size=11;
    bool opp_light=0;
    int won=0;
    unordered_set<string> choices;

    vector<vector<string> > board;
    vector<string> occupied_spaces;
    map<string,int> letter_to_int;
    vector<string> int_to_letter;

    vector<pair<string, int> > minmax;
    
    string letters[]={"a","b","c","d","e","f","g","h","i","j","k","l",
        "m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    
    for(int i=0;i<26;i++){
        letter_to_int[letters[i]]=i;
        int_to_letter.push_back(letters[i]);
    }

    int i=1;
    while(i<argv){
        if(strcmp(argc[i],n)==0){
            if((i+1)==argv){
				cerr<<"Input a size of the board for -n"<<endl;
				return 0;
			}
            else{
                size=strtod(argc[i+1],NULL);
                if(size<5 || size>26){
                    cerr<<"Board size must be between 5 and 26"<<endl;
                    return 0;
                }
                i=i+2;
            }
        }
        else if(strcmp(argc[i],l)==0){
            opp_light=1;
            i=i+1;
        }
        else{
            cerr<<"Invalid Input"<<endl;
            return 0;
        }
    }
    for (int i = 0; i < size; i++) { 
        vector<string> v;
        for (int j = 0; j < size; j++) {
            v.push_back("_");
        }
        board.push_back(v);
    } 

    int spots_left=size*size;
    string AIcolor;
    string playerColor;
    if(opp_light){
        AIcolor="X";
        playerColor="O";
        board[(size/2)][(size/2)]=AIcolor;
        string choice=int_to_letter[(size/2)]+to_string((size/2)+1);
        cout<<"Computer's Move"<<endl;
        cout<<"Move Played: "<<choice<<endl;
        occupied_spaces.push_back(choice);
        printBoard(board);
        findMyChoices((size/2),size/2,choices,int_to_letter,occupied_spaces,size,choice);
    }
    else{
        printBoard(board);
        playerColor="X";
        AIcolor="O";
    }
    while(!won){
        bool getInput=1;
        bool inputValid=0;
        int letter, position;
        //add check to see if input is valid
        while(getInput){
            string move;
            while(!inputValid){
                cout<<"Please Enter Your Move:"<<endl;
                cin>>move;
                string pos;
                if(move.size()>3 || move.size()==1){
                    cout<<"Input Invalid"<<endl;
                    continue;
                }
                else if(!isalpha(move[0]) || !isdigit(move[1])){
                    cout<<"Input Invalid: format is <letter><number"<<endl;
                    continue;
                }
                else if(move.size()==3 && !isdigit(move[2])){
                    cout<<"Input Invalid"<<endl;
                    continue;
                }
                parseMove(move,letter,position,letter_to_int);
                if(position>size || letter>size){
                    cout<<"Input Invalid: position is off board"<<endl;
                    continue;
                }
                if(board[position-1][letter]!="_"){
                    cout<<"Spot is already taken, please choose another"<<endl;
                    continue;
                }
                inputValid=1;
            }
            int won;
            board[position-1][letter]=playerColor;
            getInput=0;
            cout<<"Move Played: "<<int_to_letter[letter]<<position<<endl;
            occupied_spaces.push_back(move);
            printBoard(board);
            spots_left--;
            if(spots_left==0){
                cout<<"Game is a tie"<<endl;
                return 0;
            }
            findMyChoices(letter,position,choices,int_to_letter,occupied_spaces,size,move);
            int score=0;
            score=calculateScore(board,letter,position-1,score,playerColor,won);
            if(won==1){
                cout<<"YOU WIN!"<<endl;
                return 0;
            }
        }
        unordered_set<string>::iterator it;
        // int max_score=INT_MIN;
        int alpha=INT_MIN;
        string AIMove;

        for (it = choices.begin(); it != choices.end(); ++it) {
            int row, column;
            //vector<vector<string> > min_board=board;
            unordered_set<string> min_choices=choices;
            vector<string> new_occupied_spaces=occupied_spaces;
            new_occupied_spaces.push_back(*it);
            parseMove(*it,column,row,letter_to_int);
            // min_board[row-1][column]=AIcolor;
            board[row-1][column]=AIcolor;
            findMyChoices(column,row,min_choices,int_to_letter,new_occupied_spaces,size,*it);
            int score=0;
            score=calculateScore(board,column,row-1,score,playerColor,won);
            score=findMinMax(min_choices,board,int_to_letter,letter_to_int,playerColor,score,alpha);
            // if(it==choices.begin()){
            //     alpha=score;
            // }
            if(score>alpha){
                alpha=score;
                AIMove=*it;
            }
            // if(score>max_score){
            //     max_score=score;
            //     AIMove=*it;
            // }
            board[row-1][column]="_";
        }
    
        parseMove(AIMove,letter,position,letter_to_int);
        board[position-1][letter]=AIcolor;
        cout<<"Computer's Move"<<endl;
        cout<<"Move Played: "<<int_to_letter[letter]<<position<<endl;
        occupied_spaces.push_back(AIMove);
        printBoard(board);
        spots_left--;
        if(spots_left==0){
            cout<<"Game is a tie"<<endl;
            return 0;
        }

        findMyChoices(letter,position,choices,int_to_letter,occupied_spaces,size,AIMove);
        int score=0;
        score=calculateScore(board,letter,position-1,score,playerColor,won);
        if(won){
            cout<<"YOU LOST. LOLOLOL. TRY HARDER NEXT TIME"<<endl;
            return 0;
        }
    }
}