#include<bits/stdc++.h> 
using namespace std; 
map<string,string> optab; 
map<string,int> symtab; 

map<string,int> present_op; 
map<string,int> present_sym;  
void Set(const string &s1,const string &code) 
{ 
    optab[s1] = code;
    present_op[s1] = 1;
}
void setup_optab(){ 

Set("LDA" , "00"); 
Set("LDX" , "04"); 
Set("LDL" , "08"); 
Set("LDCH" , "50"); 
Set("STA" , "0C");
Set("STX" , "10");
Set("STL" , "14"); 
Set("STCH","54"); 
Set("SUB" , "1C");
Set("ADD" , "18"); 
Set("MUL" , "20"); 
Set("DIV" , "24"); 
Set("COMP" , "28"); 
Set("J" ,"3C"); 
Set("JLT" , "38"); 
Set("JEQ" , "30"); 
Set("JSUB" , "48"); 
Set("JGT" , "34"); 
Set("RSUB" , "4C"); 
Set("TIX" , "2C");
Set("TD" , "E0");
Set("RD" , "D8"); 
Set("WD" , "DC"); 


} 
vector<int> instruction_to_address;
vector<string> instruction_to_address_hex; 
int parse_pass1(string &s1 , vector<string> &v1) 
{ 
if(s1[0] == '.') 
return 0; 
int length = s1.size(); 
for(int i=0;i<length;++i) 
{   
    int index=i ; 
    string temp; 
    while(index< length && s1[index]!=' ') 
    { 
        temp.push_back(s1[index]); 
        index++;
    } 
    if(temp.size() > 0) 
        v1.push_back(temp);  
    else 
    { 
        v1.push_back(" ");
    }
    i= max(index, i+1);

}  

return 1; 

}
int hextodec(string &s1) { 

    int ans=0;  
    int n = s1.size();
    for(int i=0;i<n;++i) 
    { 
        ans = ans*16 + ((s1[i]<='9')?(s1[i]-'0'):(s1[i]-'A' + 10)); 

    }  
    return ans;

} 
string dectohex(int n) 
{ if(n==0) 
    return "0000";
    string s1; 
    int dec; 
    while(n!=0) 
    { 
            dec = (n%16); 
            if(dec<=9) 
            { 
                s1.push_back((char)(dec+'0'));
            } 
            else 
            { 
                s1.push_back((char)(dec - 10 +'A'));
            } 
            n=n/16;
    } 
    reverse(s1.begin(),s1.end()); 
    return s1; 

}
void convert() 
{ 
    for(auto &it: instruction_to_address) 
    { 
        instruction_to_address_hex.push_back(dectohex(it)); 

    }
}


void padd(string &input , char c , int tlen , int start = 0) 
{ 
    int n=input.size(); 
    if(n == tlen) 
        return ; 
    if(start == 1)
        reverse(input.begin() , input.end()); 
    for(int i=n;i<tlen;++i) 
    { 
        input.push_back(c);
    }  
    if(start == 1)
        reverse(input.begin(),input.end()); 
} 


int detsize(string &const1) 
{ 
    if(const1[0]=='C') 
        return (const1.size() - 3); 
    else 
        return (const1.size()-3)/2; 

} 
void parse_header(ifstream &inpfile , ofstream &outfile, int start , int length) 
{ 
    string header = "H"; 
string temp;  

getline(inpfile, temp);
vector<string> array; 
//cout<<temp<<"\n";
parse_pass1(temp,array);  
//cout<<temp<<"\n";
//cout<<"padded the line\n"; 
padd(array[0] , ' ' , 6 , 0);   
for(auto &it : array[0]) 
    header.push_back(it);  
cout<<"accessing instruction to address hex\n";
padd(instruction_to_address_hex[0] , '0' , 6 ,1); 
cout<<"accessing over\n";
for(auto &it:instruction_to_address_hex[0]) 
    header.push_back(it);  
cout<<"inserted header\n";
string hexver = dectohex(length); 
cout<<hexver<<"\n";
padd(hexver,'0',6,1); 
cout<<hexver<<"\n"; 
for(auto &it: hexver) 
    header.push_back(it);  
cout<<header<<"\n";
outfile << header<< "\n";  
cout<<"over\n";
}

string convert_const(string &inp) 
{ string result; 
  int n = inp.size();  
    if(inp[0] == 'X') 
        { 
            for(int i=2;i<n-1;++i) 
                result.push_back(inp[i]);
        }  
    else 
        {  
            for(int i=2;i<n-1;++i) 
                { 
                    string asciicode = dectohex((int)inp[i]); 
                    if(asciicode.size() == 1) 
                        padd(asciicode, '0', 2 , 1); 
                    for(auto &it: asciicode) 
                        result.push_back(it);
                }
        } 
    return result;
}

int parse_command(string &input , int& col ,string & out,int& actcol) 
{   int flag=1;
    vector<string> array; 
    parse_pass1(input,array); 
    //cout<<"in parse "<<input<<" "<<array.size()<<"\n";
    int length = 0; 
    if(array.size()<3) 
    array.push_back(" "); 
    if(array[1] == "END") 
        return -1;
    if(present_op[array[1]] == 1) 
    {
        string code = optab[array[1]]; 
        length += (code.size()/2);
        for(auto &it: code) 
            out.push_back(it); 
       if(array[2].size() == 0) 
            cout<<"not ok "<<" "<<array[2]<<"\n";
        if(array[2] == " ") 
            {   //cout<<"right\n";
                for(int i=0;i<4;++i) 
                    out.push_back('0');
                length += 2;
            } 
        else 
            {   //cout<<"wrong\n";
                for(auto &it:dectohex(symtab[array[2]])) 
                    out.push_back(it); 
                length+=2;
            }
        
    } 
    else if ( (array[1] == "WORD") || (array[1] == "BYTE"))
    { string result;
       if(array[1] == "BYTE") 
            {
            length = detsize(array[2]); 
            result = convert_const(array[2]); 
            } 
        else 
            {  length = 3;
                result = dectohex(stoi(array[2])); 
                padd(result,'0',6,1);

            }       
        //padd(result,'0',6,1);
        for(auto &it : result) 
            out.push_back(it); 
        
    } 
    else  
    {   flag=0;
        length += 3;
    }  
    
if(col + length*2 - 1 <= 69) 
    { col = col + length*2;
        if(flag == 1) 
            actcol+= (2*length);
        return  1;
    } 
else 
    {   //cout << array[1] <<"\n"; 
        
        return 0;
    }
}

int pass2(ifstream & inpfile , ofstream &outfile,int start , int length) 
{  
cout<<"entered pass2"<<"\n";
cout<<"begin header";
parse_header(inpfile,outfile,start,length); 
cout<<"header parsed!! \n"; 
string temp;
string tempout; 
int spare= 0; 
string sparestring; 

int tc = 0;
// Now parsing all the text records 
int ic = 0;
while(true) 
{ cout<< tc <<"\n";
int flag=1;
string textrecord = "T";  
if(ic >= instruction_to_address_hex.size()) 
    {cout<<"error cond\n"; 
    break;}
padd(instruction_to_address_hex[ic] , '0' ,6, 1); 
for(auto &it : instruction_to_address_hex[ic]) 
{ 
    textrecord.push_back(it);
}    
// if(tc==4) 
//     cout<<textrecord<<"\n";
// We dont know the length yet so just putting 00
for(int i=0;i<2;++i)
    textrecord.push_back('0'); 
int col = 10,actcol = 10; 
//cout<<tc<<" "<<textrecord<<"\n";
while(true) 
{   
    if(spare == 0)
        getline(inpfile,temp); 
    else 
        {temp = sparestring; 
        spare = 0;} 
    //  if(tc==5)
    //     cout<<temp<<"\n";
    int status = parse_command(temp,col,tempout,actcol);
    // if(tc == 5) 
    //     cout<<temp<<" "<<tempout<<"\n";
    if(status == -1) 
    {   tempout.clear();
        flag= 0; 
        break;
    } 
    if(status == 0) 
    {   spare= 1; 
        sparestring = temp;
        tempout.clear();
        break;
    } 
    for(auto &it:tempout) 
    { 
        textrecord.push_back(it); 
    } 
    ic++;
    tempout.clear();    
}  
//printf("length of text record: %d is  %d  \n" , tc, col - 10);
if(actcol == 10) 
    break; 
string objectlength = dectohex((actcol-10)/2);  

if(objectlength.size() == 1) 
    padd(objectlength,'0' ,2,1); 

textrecord[7] = objectlength[0]; 
textrecord[8] = objectlength[1];  

tc++;
//cout<<textrecord<<"\n";
outfile << textrecord << "\n"; 
if(flag == 0) 
    break;
} 

string endrecord = "E"; 
string hexver = dectohex(length); 
padd(hexver,'0',6,1); 
for(auto &it: hexver) 
    endrecord.push_back(it); 
outfile << endrecord <<"\n"; 

inpfile.close(); 
outfile.close();
return 0;
}
int pass1(ifstream &inpfile,ofstream &interim, int* sa = 0 , int *size = 0) 
{  
string temporary; 
vector<string> array; 
int locctr = 0; 
int flag; 
 
/// SPECIAL PARSING REQUIRED FOR FIRST LINE
getline(inpfile , temporary); 
flag = parse_pass1(temporary,array); 
//cout<<temporary<<"\n";
if(array[1] == "START") 
    {   
        *sa = hextodec(array[2]); 


        interim << temporary<<"\n"; 
        locctr = *sa;
    }
else 
    {
        locctr = 0;
    }

array.clear(); 
int count0 = 0; 

while(inpfile) 
{ 
getline(inpfile,temporary); 
flag= parse_pass1(temporary,array); 
if(flag == 0) 
    {array.clear();
    continue;}    
instruction_to_address.push_back(locctr); 

//cout<<temporary<<"\n"; 
//count0++;
if(array[1] == "END") 
    {   interim << temporary<<"\n"; 
        break;}
if(array[0] == " ") 
    {  
        // no label
    }  
else 
    { //cout << array[0]<<"\n";
        if(present_sym[array[0]] == 1) 
            cout<<"error at 0 " <<array[0]<<"\n"; 
        else 
            {present_sym[array[0]] = 1; 
            symtab[array[0]] = locctr;}
    }   
    //cout<<array[1]<<"\n"; 
    std::cout << count0<<" "<<locctr<<" "<<dectohex(locctr)<<" "<<array[1]<<"\n";
if (present_op[array[1]] == 1) 
    { 
        locctr+=3;
    } 
else if (array[1] == "WORD") 
    { 
        locctr+=3;
    } 
else if (array[1] == "RESW") 
    {    
        //    cout<<count0<<" " <<array[2]<<" "<<array[0]<<" "<<stoi(array[2])<<" "<<locctr<<"\n";
        
        locctr+= 3*stoi(array[2]);
    } 
else if(array[1] == "RESB") 
    { // replace this 
        if(count0 == 19) 
        { 
          //  cout<<count0<<" " <<array[2]<<" "<<stoi(array[2])<<"\n";
        }
        locctr += stoi(array[2]);
    } 
else if(array[1] == "BYTE") 
    {   // replace this 
        cout<<array[2]<<"\n";
        locctr += detsize(array[2]);
    } 
else 
    { 
        cout<<"error at 1 "<<array[1]<<"\n";
    } 
array.clear(); 
count0++;
interim << temporary <<"\n";

}  
interim.close(); 
inpfile.close(); 
*size = (locctr - *sa);
return (locctr - *sa); 

}



int main() { 
ifstream inpfile; 
inpfile.open("input.txt"); 
ofstream interim; 
interim.open("intermediate.txt"); 
int sa = 0, size =0 ; 
setup_optab();
pass1(inpfile,interim,&sa,&size);  

cout<<sa<<" "<<size<<"\n"; 
convert();
cout<<"opening intermediate.txt\n";
ifstream intread("intermediate.txt"); 
cout<< "opening output.txt\n"; 
ofstream aoutput("output.txt"); 
pass2(intread,aoutput,sa,size);


}