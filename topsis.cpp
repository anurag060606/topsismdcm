#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
using namespace std;

void takeInput(vector<string>&option_names_vector,vector<string>&parameters_vector,vector<string>&impact_vector,vector<double>&parameter_weight_vector,int &rows,int &columns,vector<vector<double>> &matrix)
{
    cout<<"Taking input for"<<"\033[1;33m"<<" options: \n"<<"\033[0m";
    for(int i=1;i<=columns;i++){
        string option_name;
        cout<<"\tEnter the "<<i<<" option: ";
        cin>>option_name;
        option_names_vector.push_back(option_name);
    }
    cout<<"\n\nTaking input for"<<"\033[1;33m"<<" parameters: \n"<<"\033[0m";
    double weight_sum=0;
    for(int i=1;i<=rows;i++){
        string parameter;
        double weight;
        string impact;
        cout<<"\tEnter the "<<i<<" parameter: ";
        cin>>parameter;
        cout<<"\tWeight for the parameter (Remaining capacity : "<<1-weight_sum<<"): ";
        cin>>weight;
        cout<<"\tImpact of the parameter(+\\-): ";
        cin>>impact;
        weight_sum=weight_sum+weight;
        parameters_vector.push_back(parameter);
        parameter_weight_vector.push_back(weight);
        impact_vector.push_back(impact);
        cout<<endl;
    }

    
    for(int i=1;i<=columns;i++){
        cout<<"Enter values for option: "<<"\033[1;33m"<<option_names_vector[i-1]<<"\033[0m\n";
        for(int j=1;j<=rows;j++){
            double value;
            cout<<"\tParameter->"<<"\033[1;35m"<<parameters_vector[j-1]<<"\033[0m : ";
            cin>>value;
            matrix[j-1][i-1]=value;

        }
        cout<<endl;
    }

}
void normalise(vector<vector<double>> &matrix,int &rows,int &columns,vector<vector<double>> &normalised_matrix,vector<double>parameter_weight_vector)
{
    for(int i=1;i<=rows;i++){
        double denosquared=0;
        for(int j=1;j<=columns;j++){
            denosquared=denosquared+(matrix[i-1][j-1])*(matrix[i-1][j-1]);
        }
        for(int j=1;j<=columns;j++){
            normalised_matrix[i-1][j-1]=((matrix[i-1][j-1])/sqrt(denosquared))*parameter_weight_vector[i-1];
        }
    }
}
void idealVal(vector<vector<double>>normalised_matrix,vector<double>&idealbestvector,vector<double>&idealworstvector,int rows,int columns,vector<string>impact_vector)
{
    for(int i=1;i<=rows;i++){
        double ideal_best=normalised_matrix[i-1][0];
        double ideal_worst=normalised_matrix[i-1][0];
        if(impact_vector[i-1]=="+"){
            for(int j=2;j<=columns;j++){
                if(normalised_matrix[i-1][j-1]>ideal_best){
                    ideal_best=normalised_matrix[i-1][j-1];
                }
            }
            for(int j=2;j<=columns;j++){
                if(normalised_matrix[i-1][j-1]<ideal_worst){
                    ideal_worst=normalised_matrix[i-1][j-1];
                }
            }
            idealbestvector.push_back(ideal_best);
            idealworstvector.push_back(ideal_worst);
        }else if(impact_vector[i-1]=="-"){
            for(int j=2;j<=columns;j++){
                if(normalised_matrix[i-1][j-1]<ideal_best){
                    ideal_best=normalised_matrix[i-1][j-1];
                }
            }
            for(int j=2;j<=columns;j++){
                if(normalised_matrix[i-1][j-1]>ideal_worst){
                    ideal_worst=normalised_matrix[i-1][j-1];
                }
            }
            idealbestvector.push_back(ideal_best);
            idealworstvector.push_back(ideal_worst);
        }else{
            cout<<"\033[1;31m"<<"FATAL ERROR: UNIDENTIFIED IMPACT CHARACTER ENTERED DURING EARLIER STAGE";
        }
    }
}
void idealdiscalc(vector<vector<double>>normalised_matrix,vector<double>idealbestvector,vector<double>idealworstvector,vector<double>&idealbestdisvector,vector<double>&idealworstdisvector,int rows,int columns)
{
    for(int j=1;j<=columns;j++){
        float idealbestdis=0;
        float idealworstdis=0;
        for(int i=1;i<=rows;i++){
            idealbestdis=idealbestdis+(normalised_matrix[i-1][j-1]-idealbestvector[i-1])*(normalised_matrix[i-1][j-1]-idealbestvector[i-1]);
            idealworstdis=idealworstdis+(normalised_matrix[i-1][j-1]-idealworstvector[i-1])*(normalised_matrix[i-1][j-1]-idealworstvector[i-1]);
        }
        idealbestdis=sqrt(idealbestdis);
        idealworstdis=sqrt(idealworstdis);
        idealbestdisvector.push_back(idealbestdis);
        idealworstdisvector.push_back(idealworstdis);
    }
}
void score(vector<double>&topsis_score_vector,vector<double>idealbestdisvector,vector<double>idealworstdisvector,int columns,int rows){
    for(int i=1;i<=columns;i++){
        topsis_score_vector.push_back(idealworstdisvector[i-1]/(idealbestdisvector[i-1]+idealworstdisvector[i-1]));
    }
}


int main() {
    cout<<"This is a implementation of "<<"\033[4m"<<"Technique for Order Preference by Similarity to Ideal Solution (TOPSIS)"<<"\033[0m"<<" as a Multi Criteria Decision Making (MCDM) methodology in CPP. Read documentation for more information.";
    int columns,rows;
    cout<<"\n\n\nHow many options are you considering?: ";cin>>columns;
    cout<<"On how many parameters are you basing your evaluation?: ";cin>>rows;
    cout<<"\n\n";
    vector<string>option_names_vector,parameters_vector,impact_vector;
    vector<double>parameter_weight_vector;
    vector<vector<double>> matrix(rows, vector<double>(columns));
    vector<vector<double>> normalised_matrix(rows, vector<double>(columns));
    vector<double>idealbestvector,idealworstvector;
    vector<double>idealbestdisvector,idealworstdisvector,topsis_score_vector;
    takeInput(option_names_vector,parameters_vector,impact_vector,parameter_weight_vector,rows,columns,matrix);
    normalise(matrix,rows,columns,normalised_matrix,parameter_weight_vector);
    idealVal(normalised_matrix,idealbestvector,idealworstvector,rows,columns,impact_vector);
    idealdiscalc(normalised_matrix,idealbestvector,idealworstvector,idealbestdisvector,idealworstdisvector,rows,columns);
    score(topsis_score_vector,idealbestdisvector,idealworstdisvector,columns,rows);
    cout<<"Normalised matrix: \n\n";
    for(auto x:normalised_matrix){
        for(auto y:x){
            cout<<y<<" ";
        }
        cout<<endl;
    }
    cout<<"\nIdeal best vector:\n";
    for(auto x:idealbestvector){
        cout<<x<<" ";
    }
    
    cout<<"\n\nIdeal worst vector:\n";
    for(auto x:idealworstvector){
        cout<<x<<" ";
    }
    cout<<"\n\nIdeal best distance:\n";
    for(auto x:idealbestdisvector){
        cout<<x<<" ";
    }
    cout<<"\n\nIdeal worst distance:\n";
    for(auto x:idealworstdisvector){
        cout<<x<<" ";
    }
    cout<<"\n\nTopsis score: \n";
    for(int i=0;i<option_names_vector.size();i++){
        cout<<"Option: "<<"\033[1;34m"<<option_names_vector[i]<<"\033[0m"<<" : "<<"\033[1;36m"<<topsis_score_vector[i]<<"\033[0m";
        cout<<endl;
    }
    cout<<"\033[1;32m"<<"Optimal choice: "<<option_names_vector[max_element(topsis_score_vector.begin(),topsis_score_vector.end())-topsis_score_vector.begin()]<<"\033[0m"<<endl;
    cout<<"\033[1;31m"<<"Worst choice: "<<option_names_vector[min_element(topsis_score_vector.begin(),topsis_score_vector.end())-topsis_score_vector.begin()]<<"\033[0m";
}
