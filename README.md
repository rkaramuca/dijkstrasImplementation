-------------------------------
Project 4: Dijkstra's Algorithm
-------------------------------

Goal
=============================================================================================================
Demonstrate the use of the Graph clas and output a table of the shortest pathes from 
a user-input vertex using Dijkstra's Algorithm.

Client Algorithm
=============================================================================================================
The client file will be implementing the C++ modules: 
* algorithm:
    * used for the find function in vectors
* climits:
    * used for the temporary variable INT_MAX
* fstream:
    * used for the file input/output
* iostream:
    * used for general input/output
* sstream:
    * used to split the file's lines by semicolons
* string:
    * used for string objects to hold file contents
* vector:
    * used for dynamic storage of file values
* graph.h:
    * includes graph functions given by Dr. Digh

The main function of the program is used for getting a 
valid input from the user, as well as calling the buildGraph
function and the doDijkstras function.

The client file also implements findIndex(find the index
of the parameter string in the vertices vector), createArrs(
fills the 4 parallel arrays with generic values), printRow(
prints the data for each vertex in the vertices vector), findMin(
finds the minimum, unmarked vertex in the marked vector), sortVertices(
sorts the vertices list for display when the user enters a file), 
sortAll(sorts the 4 parallel arrays, by the weights, in ascending order),
and doDijkstras(main Dijkstra's Algorithm logic flow).

The doDijkstras function goes through each vertex in the vertices list from the input file, 
then look at their adjacent vertices looking for the least-cost next vertex. It then 
marks that vertex, sets its distance/weight, as well as the previous vertex. Once this 
occurs for all vertices obtained from file, it then sorts using sortAll, then prints 
out the summary table for each vertex, printing the vertex, distance, and its origin point using printRow. 

Graph Algorithm
=============================================================================================================
The graph class is used mainly for the GetToVertices, AddVertex, and AddEdge methods.
These are called in the client file in the buildGraph method. 
* GetToVertices:
    * takes in a vertex and a queue, and fills the queue with the adjacent vertices of that original vertex
* AddVertex:
    * takes in a vertex value, and adds taht node to the graph without an edge
* AddEdge:
    * takes in a an origin vertex, destination vertex, and an edge weight value, and creates a link between the origin and destination with distance "weight"

Sample Input/Output
=============================================================================================================
With a file containing 3 lines of data...
* V1;V2;7
* V2;V3;8
* V3;V4;10

...the program will first check the vertices in the file to see if they are in the 
vertices vector. If they aren't, it'll add the vertex to the vector as well as the graph. 

It will then get the weight and vertex pairs, and create edges based on it:
* AddEdge(V1, V2, 7);
* etc...

Next, it will assign the correct values for the starting vertex, given by the user, 
in the parallel arrays:
* vertices[0] = "V1";
* marked[0] = true;
* distance[0] = 0;
* previous[0] = "N/A";

Lastly, it will repeat that same assignments for each vertex, but the weight, marked, and previous 
will be decided based upon which next vertex has the least cost. The least-cost gets a true 
mark, distance is the sum weight from the beginning to that point, and previous is the last node it came from. 

At the end, it will sort the vectors based on the distance, in ascending order, and print 
out a table of information for each vertex:
* Enter a starting vertex: V1
* 
* Vertex----Distance-----Previous
*     V1           0         N/A
*     V2           7          V1
*     V3          15          V2
*     V4          25          V3