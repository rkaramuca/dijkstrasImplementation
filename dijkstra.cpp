/*
 * Ronald Karamuca
 * Dijkstra's Algorithm implementation on a di-graph of vertices/edge weights.
 * December 2020
 * FOR CSC245: DATA STRUCTURES AND ALGORITHMS. 
*/

#include <algorithm>    // used for "find" in vectors
#include <climits>      // used for INT_MAX 
#include <fstream>      // used for file IO
#include <iostream>     // used for general IO
#include <sstream>      // used to split input lines by semicolon
#include <string>       // used for string objects
#include <vector>       // used for dyanmic storage vectors
#include "graph.h"      // includes graph functions given by Dr. Digh
using namespace std;

// function prototypes for various graph utilities
void buildGraph(Graph<string>& myGraph, string fileName, vector<string> &vertices);
int findIndex(vector<string> vertices, int numVertices, string toFind);
void createArrs(vector<string> vertices, vector<bool>& marked, vector<int>& distance, vector<string>& prev, int numVertices);
void printRow(string origin, int distance, string previ);
int findMin(vector<bool>& marked, vector<int>& distance);
void sortVertices(vector<string> vertices);
void sortAll(vector<string>& vertices, vector<bool>& marked, vector<int>& dist, vector<string>& prev);
void doDijkstras(Graph<string> graph, vector<string> vertices);

// main logic flow
int main(int argc, char *argv[])
{
    // file I/O: check if the filename is given, if it is, check if it's correct
    string fileName;
    if (argv[1] != NULL)
        fileName = argv[1];
    else
    {
        cout << "Bad input, include a file name.\nTry again." << endl;
        return 0;
    }

    ifstream infile;
    infile.open(fileName);
    if (!infile)
    {
        cout << "File name is incorrect.\nTry again." << endl;
        infile.close();
        return 0;
    }
    infile.close();

    // create a vector that holds all vertices
    // and a graph with all vertices
    vector<string> vertices;
    Graph<string> myGraph;
    // build the graph using the empty graph, file IO, and vertices vector
    buildGraph(myGraph, fileName, vertices);
    
    // send off the graph and vertices vector to the main Dijkstr's function 
    doDijkstras(myGraph, vertices);

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// builds the graph with the given file contents
void buildGraph(Graph<string>& myGraph, string fileName, vector<string> &vertices)
{
    // open file stream with valid user input file
    ifstream infile;
    infile.open(fileName);

    // get all lines in the file, one by one:
    string line;
    while(getline(infile, line))
    {
        stringstream ss(line);
        string curr;
        // temporary vector to hold 3 parts of each input line -> origin, desination, and weight
        vector<string> pieces;
        // cuts the input line into 3 parts
        while(getline(ss, curr, ';'))
        {
            pieces.push_back(curr);
        }
        // builds the graph and vertices vector:
        // if the origin is not already in the vertices vector, 
        // add it to the graph and vertices vector
        if(find(vertices.begin(), vertices.end(), pieces[0]) == vertices.end())
        {
            myGraph.AddVertex(pieces[0]);
            vertices.push_back(pieces[0]);
        }
        // if the destination is not already in the vertices vector,
        // add it to the graph and vertices vector
        if(find(vertices.begin(), vertices.end(), pieces[1]) == vertices.end())
        {
            myGraph.AddVertex(pieces[1]);
            vertices.push_back(pieces[1]);
        }
        // add the edge connecting the origin and destination
        myGraph.AddEdge(pieces[0], pieces[1], stoi(pieces[2]));

        // empty out pieces for the next line of the input file
        pieces.clear();
    }
}
////////////////////////////////////////////////////////////////////////////////
// find the index of the sent in string in the vertices vector
// if it does not exist, return -1
int findIndex(vector<string> vertices, int numVertices, string toFind) 
{
    int index;

    // search the vector to look for the given vertex value, 
    // then return the index value of that vertex. 
    for(int i = 0; i < numVertices; i++) 
    {
        if(vertices[i].compare(toFind) == 0) 
        {
            index = i;
            return index;
        }
    }
    
    // if it's not found, return a -1
    return -1;
}
////////////////////////////////////////////////////////////////////////////////
// fills the 4 parallel arrays with generic values
void createArrs(vector<string> vertices, vector<bool>& marked, vector<int>& distance, vector<string>& prev, int numVertices) 
{
    // fill all arrays with generic values EXCEPT for the vertices list
    for(int i = 0; i < numVertices; i++) 
    {
        // marked(bool) gets false
        marked.push_back(false);
        // distance(int) gets INT_MAX, because it'll look for min later on
        distance.push_back(INT_MAX);
        // prev(string) gets a random value that'll be changed later
        prev.push_back("---");
    }
}
////////////////////////////////////////////////////////////////////////////////
// prints the row of data given the origin vertex, distance weight, and previous
void printRow(string origin, int distance, string previ)
{
    // checks if distance is still INT_MAX (unreachable)
    if(distance == 2000000000)
        // print Not on Path for weight and N/A for the previous vertex
        printf("%30s %30s %30s \n", origin.c_str(), "Not on Path", "N/A");
    else
        // otherwise print everythin as-is
        printf("%30s %30d %30s \n", origin.c_str(), distance, previ.c_str());
    
}
////////////////////////////////////////////////////////////////////////////////
// finds the minimum, unmarked vertex in the marked vector 
int findMin(vector<bool>& marked, vector<int>& distance)
{
    int lowIndex;  
    bool found = false;
    int i = 0;
    // find a generic lowIndex value to start the comparison... 
    // MUST NOT BE MARKED(used) ALREADY
    while(!found && i < distance.size())
    {
        // if we find one, set lowIndex to the index found, and break the loop
        if(marked[i] == false) 
        {
            lowIndex = i;
            found = true;
        } 
        i++;
    }

    // now we go through with that unmarked value index and see if we can find
    // a smaller, unmarked distance value
    i = 1;
    while(i < marked.size())
    {
        if(distance[i] < distance[lowIndex] && marked[i] == false)
            lowIndex = i;
        i++;
    }

    // return that index
    return lowIndex;
}
////////////////////////////////////////////////////////////////////////////////
// sorts the vertices list for display when the user enters a file
void sortVertices(vector<string> vertices)
{
    // sort alphabetically
    sort(vertices.begin(), vertices.end());
    int numVertices = vertices.size();

    // clear screen then print the "title screen"
    system("clear");
    cout << "\t\t--------------- Dijkstra's Algorithm Program ---------------\n\n";
    cout << "A Weighted Graph has been Build for these " << numVertices << " Cities:\n" << endl;

    // logic for printing in 3 column format
    int counter = 0;
    for(int i = 0; i < numVertices; i++) 
    {
        if(counter % 2 == 0 && counter != 0)
        {
            // if it's the third item in the row, print it with a newline
            printf("%30s \n", vertices[i].c_str());
            counter = 0;
        }
        else 
        {
            printf("%30s", vertices[i].c_str());
            counter++;
        }
    }  
}
////////////////////////////////////////////////////////////////////////////////
// sorts all 4 parallel arrays 
void sortAll(vector<string>& vertices, vector<bool>& marked, vector<int>& dist, vector<string>& prev)
{
    // bubble sort based on the distance values
    int dSize = dist.size();
    for(int i = 0; i < dSize; i++)
    {
        for(int j = 0; j < dSize - i - 1; j++)
        {
            // if we swap distances, swap the other 3 arrays as well
            if(dist[j] > dist[j+1])
            {
                string verTemp = vertices[j];
                vertices[j] = vertices[j+1];
                vertices[j+1] = verTemp;

                bool marTemp = marked[j];
                marked[j] = marked[j+1];
                marked[j+1] = marTemp;

                int disTemp = dist[j];
                dist[j] = dist[j+1];
                dist[j+1] = disTemp;

                string preTemp = prev[j];
                prev[j] = prev[j+1];
                prev[j+1] = preTemp;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
// main Dijkstra's Algorithm logic flow
void doDijkstras(Graph<string> graph, vector<string> vertices) 
{
    // diplay all cities to the user
    sortVertices(vertices);

    // initialize blank vectors
    vector<bool> marked;
    vector<int> distance;
    vector<string> prev;
    
    // create the queue and the number of vertices
    Queue<string> graphQueue;
    int numVertices = vertices.size();

    // ask user for starting vertex and check if it's valid.
    // if it's not, keep asking.
    string startingPoint = "";
    bool found = false;
    while(!found) 
    {
        cout << "\nEnter a starting vertex: ";
        getline(cin, startingPoint);
        for(int i = 0; i < numVertices; i++) 
        {
            if(findIndex(vertices, numVertices, startingPoint) != -1) 
            {
                found = true;
                break;
            }
            else 
            {
                cout << "Not a vertex... Try again." << endl;
                break;
            }
        }
    }

    // starting index based on the position of the input vertex 
    // in the vertices array
    int startIndex = findIndex(vertices, numVertices, startingPoint);
    graph.MarkVertex(startingPoint);

    // create the adjacent arrays with default values
    createArrs(vertices, marked, distance, prev, numVertices);

    // update adjacent arrays for the starting vertex
    marked[startIndex] = true;
    distance[startIndex] = 0;
    prev[startIndex] = "N/A";

    // for all subsequent vertices...
    for(int i = 0; i < numVertices - 1; i++) 
    {
        // find the current vertex's adjacent vertices
        graph.GetToVertices(startingPoint, graphQueue);
        vector<string> adjacents;
        // add the queue vertices to the adjacents vector
        while(!graphQueue.isEmpty())
        {
            adjacents.push_back(graphQueue.dequeue());
        }

        // check each adjacent point
        for(int i = 0; i < adjacents.size(); i++) 
        {
            // if it's not marked...
            if(!graph.IsMarked(adjacents[i])) 
            {
                // get its weight then add it to the previous weights, 
                // leading up to that vertex, then compare it to the distance
                // that is already there. Should be INT_MAX if it's not marked.
                int currIndex = findIndex(vertices, numVertices, adjacents[i]);
                int weight = graph.WeightIs(startingPoint, adjacents[i]) + distance[startIndex];
                if( weight < distance[currIndex]) 
                {
                    // set the vertex's origin point/previous vertex and
                    // its new weight value
                    prev[currIndex] = startingPoint;
                    distance[currIndex] = weight;
                }
            }
        }  
        // empty the adjacent list for the next loop
        adjacents.empty();
        // new starting index is the next min index in the vector
        startIndex = findMin(marked, distance);
        // mark the value at the new index, and set the new starting 
        // point to the vertex at the new index in the vertices vector 
        marked[startIndex] = true;
        startingPoint = vertices[startIndex];
    }
    // print out the fancy formatted vertex, distance, and previous vectors
    cout << "--------------------------------------------------------------------------------------------\n";
    printf("%30s %30s %30s \n", "Vertex", "Distance", "Previous");
    printf("%30s %30s %30s", "------", "--------", "--------");

    // checks if the values are still +-INT_MAX, if they are, 
    // set the distance to an unreasonable value
    cout << endl;
    for(int i = 0; i < numVertices; i++)
        if(distance[i] > 2000000000 || distance[i] < 0) 
            distance[i] = 2000000000;

    // sorts the parallel vertices, marked, weight, and origin vectors 
    sortAll(vertices, marked, distance, prev);
    // print them all with the respective data
    for(int i = 0; i < numVertices; i++)
        printRow(vertices[i], distance[i], prev[i]);
}