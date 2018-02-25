/*
--Text RLE bit level algorithm--
*/
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;



// Encode function taking number of zeros (int) and return binary representation as string (groups of 4 bits)
string Encode(int zeros){
    string result = "";
    int rem = zeros % 15 , quote = zeros / 15;
    if(zeros < 15)
        result = bitset<4>(zeros).to_string();
    else{
        while(quote){
            result += bitset<4>(15).to_string();
            --quote;
        }
        result += bitset<4>(rem).to_string();
    }

    return result; // result here contains number of zeros in binary string
}



// compression function using RLE on bits Algorithm takes a string in binary representation and return it compressed.
string compress(string text_in_binary){
    string compressed_text = "";
    int zero_counter = 0;

    // if the text_in_binary starts with zero
    if(text_in_binary[0] == '0'){
        for(int i = 0 ; i < text_in_binary.length();++i){
            if(text_in_binary[i] == '0')
                    ++zero_counter;
            else
                break;
        }
        compressed_text += Encode(zero_counter); // call Encode function
        //compressed_text += " ";
    }
    // if the binary text starts with 1 the compressed string should start with 0000
    else if (text_in_binary[0] == '1')
        compressed_text += "0000";

    // counting zeros in the rest of the binary text
    for(int i = 0 ; i < text_in_binary.length();++i){
        if(text_in_binary[i] == '1'){
            zero_counter = 0;
            for(int j = i + 1 ; j < text_in_binary.length();++j){
                if(text_in_binary[j] == '0')
                    ++zero_counter;
                else
                    break;
            }
            compressed_text += Encode(zero_counter);
            //compressed_text += " ";
        }
    if(text_in_binary[text_in_binary.length() - 1] == '1')
        compressed_text += "0000";
    }
    return compressed_text; // return the compressed text
}


// converting string represent a binary number and return integer
int convert_to_int(string x){
    int n = 0 , j = -1 , l = x.length();
    while (++j < l)
        n = (n << 1) + (x[j] == '0' ? 0 : 1);
    return n;
}


// decompression function using RLE on bits Algorithm takes a compressed string and return the original one.
string decompress(string compressed_text){
    string original_text , temp = "";
    int cnt = 0;

    for(int i = 0 ; i < compressed_text.length();i += 4){
        temp = ""; // read the 4 group
        for(int j = 0 ; j < 4 ; ++j)
            temp += compressed_text[i + j];
        cnt += convert_to_int(temp);
        if(cnt % 15 != 0){
            for(int i = 0 ; i < cnt ;++i)
                original_text += "0";
            original_text += "1";
            cnt = 0;
        }
        else {
            string next = ""; // next 4 bit group in case if zeros equal 15 or greater
            for(int j = i + 4 ; j < i + 8 ; ++j)
                next += compressed_text[j];
            if(next == "0000"){
                for(int i = 0 ; i < cnt ;++i)
                    original_text += "0";
                if(cnt != 15)
                    original_text += "1";
                cnt = 0;
            }
            // for the case two ones having no zeros between them like (11....)
            else if(!cnt)
                original_text += "1";

        }


    }
    // if the compressed text starts or ends with 1
    original_text.pop_back();
    return original_text;
}


int main()
{
    freopen("input.txt","r" ,stdin);
    freopen("output.txt" ,"w",stdout);
    ios::sync_with_stdio(0);cin.tie(0),cout.tie(0);

    string line, text, text_in_binary = "" ;

    while(getline(cin,line)){
        text += line;
        text.push_back('\n');
    }

    //cout << text << endl ;
    int z = 0 , o = 0;
    for(int i = 0 ; i < text.length();++i){
       // if(text[i] != '\n')
            text_in_binary += bitset<8>(text[i]).to_string()  ;
    }


    string compressed_text = compress(text_in_binary); // calling the compressing function
    string original_text   = decompress(compressed_text); // calling decompression function

    int compressed_text_length = compressed_text.length();
    int original_text_length   = original_text.length() ;

    // calculating the compression ration ( compressed_length / original_length ) % 100
    float compression_ratio = (float)compressed_text_length / original_text_length;

    if(text_in_binary == original_text){
        cout << setprecision(3) <<  "successful compression with compression ratio = " << compression_ratio << " %" << endl << endl;

        string char_in_binary = "" , text_after_decompression;


    // getting the original text after decompression
    for(int i = 0 ; i < original_text_length ;i+=8){
        char_in_binary = ""; // every character was encoded in 8 bit binary using bitset<8>
        for(int j = i ; j < i+8 ; ++j)
            char_in_binary += original_text[j];

        char c = strtol(char_in_binary.c_str(), 0, 2); // encoding the binary to corresponding letter
        text_after_decompression += c ;
    }

    cout <<  "The original text after decompression  is :" << endl << endl ;
    cout << text_after_decompression << endl;

        if(compression_ratio >= 1.0)
            cout << "Caution! this algorithm is not useful for this text." << endl ;

    }
    else{
        cout << "Error!" << endl;
    }

    return 0;
}
