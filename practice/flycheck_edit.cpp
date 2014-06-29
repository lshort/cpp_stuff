#include <string>
#include <vector>
#include <iostream>


using namespace std;


int edit_distance( string a, string b )
{
    int m = a.size();
    int n = b.size();
    //vector <int> sample(m+1,0);
    //vector<vector<int>> matrix(n+1,sample);

    vector<vector<int>> matrix(n+1,vector<int>(m+1,0));

    for (int i=0; i<=n; ++i)
        matrix[i][0] = i;
    for (int k=0; k<=m; ++k)
        matrix[0][k] = k;

    for (int i=1; i<=n; ++i)
    {
        for (int k=1; k<=m; ++k)
        {
            if (b[i-1]==a[k-1])
                matrix[i][k] = matrix[i-1][k-1];
            else
                matrix[i][k] = min( matrix[i-1][k-1]+1,
                                    min( matrix[i-1][k]+1,matrix[i][k-1]+1 ));
        }
    }
    return matrix[n][m];
}



int main()
{
    cout << edit_distance("kitten","sitting") << endl;
    cout << edit_distance("","sitting") << endl;



}
