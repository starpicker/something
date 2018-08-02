#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef pair<double, string> PAIR;

ostream& operator << (ostream& out, const PAIR& p)
{
    if(p.first > 5.0000)
        return out /*<< p.first << "\t" */<< p.second;
}

int main()
{
    map<double, string, greater<double>> rating_file_map;
    
    ifstream rating_file("predictions.txt");
    ifstream file_list_file("face_landmark_file_list.txt");
    ofstream output_file("selected_file_list.txt");
    
    string str_rating;
    string str_file_list;
    
    while(getline(rating_file, str_rating) && getline(file_list_file, str_file_list))
    {
        stringstream ss;
        ss << str_rating;
        double data;
        ss >> data;
        rating_file_map.insert(make_pair(data, str_file_list));
    }
    
    for(auto iter: rating_file_map)
    {
        output_file << iter << endl;
    }
    
    rating_file.close();
    file_list_file.close();
    output_file.close();
    
    return 0;
}