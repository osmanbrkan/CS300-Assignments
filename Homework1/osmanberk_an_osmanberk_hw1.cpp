//
//  main.cpp
//  cs300_homework1
//
//  Created by Osman Berk An on 24.10.2023.
//

#include "Stack.hpp"
#include "Stack.cpp"
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <string>

using namespace std;


//
//Maze Generating
//


void BreakingWalls(Cell& old, Cell &target){   // wall breaking between the cells func
    
    //dont checking is there a wall exist between two cell or not, just breaking the wall function
    
    
    int dx = target.coorX - old.coorX;
    int dy = target.coorY - old.coorY;
    
    if (dx == 1) {
        target.l = old.r = 0; // Break left wall of target and right wall of old
    }
    
    else if (dx == -1) {
        target.r = old.l = 0; // Break right wall of target and left wall of old
    }
    
    else if (dy == 1) {
        target.d = old.u = 0; // Break down wall of target and up wall of old
    }
    
    else if (dy == -1) {
        target.u = old.d = 0; // Break up wall of target and down wall of old
    }
    
    target.visited = true;  //target Cell visited
}


void MazeGenerate(int row, int column, int k){  //K random MXN mazes //in this function I implement random mazes according to sizes  //generating maze and mazefile 
    
    //
    unsigned int seed = static_cast<unsigned int>(time(nullptr) + k);  //changeging seed number for every mazeID guarantees to generate different maze each time. I learned that syntax shoud be like that
    srand(seed);
    //
    
    
    vector<vector<Cell>> maze(column, vector<Cell>(row));  //my maze with cells
    
    // indexing is reversed way
    for (int i = 0; i < column; i++) {
        for (int j = 0; j < row; j++) {
            
            //giving indexes
            maze[i][j].coorX = i;
            maze[i][j].coorY = j;
            
            //all walls initialize true
            maze[i][j].u = true;
            maze[i][j].d = true;
            maze[i][j].l = true;
            maze[i][j].r = true;
            
            maze[i][j].visited = false;   // all cells need to initialize not visited
        }
    }
   
    int WallBreakLeft = (row*column)-1;  //needs to break that many wall in total written in pdf
    Stack<Cell> mazeStack;
    mazeStack.push(maze[0][0]);  // push starting cell [0][0] into stack
    maze[0][0].visited = true;
    
    Cell currCell = maze[0][0];
    int count = 0;
    
    while(count != WallBreakLeft){  // there is still wall to break
        currCell = mazeStack.top();
        int X = currCell.coorX;  //top cells coordinates
        int Y = currCell.coorY;
        
        vector<Cell> unvisitedNeighbors;  // I store unvisited neighbors for selecting one of them randomly to move and break wall
        
        //checking unvisited neighbors, then randomly choose one of them and break the wall between them
        
        // checking Right neighbor for move
        int right_x = X + 1;  //right neighbors coordinates
        int right_y = Y;
        if ( (right_x >= 0 && right_x < column) && (maze[right_x][right_y].visited == false)){
            unvisitedNeighbors.push_back(maze[right_x][right_y]);  //visitable cell
        }
        // checking Left move
        int left_x = X - 1;  //x coordinate after the left move
        int left_y = Y;
        if ( (left_x >= 0 && left_x < column ) && (maze[left_x][left_y].visited == false)) {
            unvisitedNeighbors.push_back(maze[left_x][left_y]); // visitable cell
        }
        // checking Up move
        int up_x = X;
        int up_y = Y+1;  // y coordinate after the up move
        if ( (up_y >= 0 && up_y < row ) && (maze[up_x][up_y].visited == false)) {
            unvisitedNeighbors.push_back(maze[up_x][up_y]); // visitable cell
        }
        // checking Down move
        int down_x = X;
        int down_y = Y-1; // y coordinate after the down move
        if ( (down_y >= 0 && down_y < row ) && (maze[down_x][down_y].visited == false)) {
            unvisitedNeighbors.push_back(maze[down_x][down_y]); // visitable cell
        }
        if(!unvisitedNeighbors.empty()){ //if there is a possible movable cell neighbor
            int randomNeighborInd = rand() % unvisitedNeighbors.size();  // I learned that it gives random index for my unvisitedNeighbors vector
        
            //maze[unvisitedNeighbors[randomNeighborInd].coorX][unvisitedNeighbors[randomNeighborInd].coorY];  //random target cell coordinates
            //maze[mazeStack.top().coorX][mazeStack.top().coorY]; // currentCell coordinates
            
         
            
            //Knock down the wall and add the new cell to the stack.
            BreakingWalls(maze[mazeStack.top().coorX][mazeStack.top().coorY], maze[unvisitedNeighbors[randomNeighborInd].coorX][unvisitedNeighbors[randomNeighborInd].coorY]);
            
            mazeStack.push(maze[unvisitedNeighbors[randomNeighborInd].coorX][unvisitedNeighbors[randomNeighborInd].coorY]);
            
            count++;   // One more wall breaked
            
        }//If no wall exists to knock down for the currentCell, backtrack using the stack until you find a cell which has an unvisited neighbor cell.
        
        else
            mazeStack.pop();
    
        
    }
   //generate a maze_K.txt file
    ofstream mazeFile;
 
    string filename = "maze_" + to_string(k) + ".txt";  //kth maze //maze_1.txt
    
    mazeFile.open(filename);
   
    if(mazeFile.is_open()){
        mazeFile<< row << " " << column << endl;   //first row
        
        
        /*x=0,y=0,l=1,r=0,u=1,d=1
        x=1,y=0,l=0,r=0,u=1,d=1
        x=2,y=0,l=0,r=0,u=0,d=1
        x=3,y=0,l=0,r=0,u=1,d=1
        x=4,y=0,l=0,r=1,u=1,d=1*/
        
        for(int i=0; i<row; i++){  //other rows written like that
            for(int j=0; j<column; j++){
                mazeFile << "x=" << maze[j][i].coorX << " y=" << maze[j][i].coorY << " l=" << maze[j][i].l << " r=" << maze[j][i].r << " u=" << maze[j][i].u << " d=" << maze[j][i].d << endl;
            }
        }
    }
    
    mazeFile.close();

}


//
//Path Finding
//

void PathFinding(int row, int column, int mazeID, int entryX, int entryY, int exitX, int exitY){
    //find path for the maze and generate a mazepath file
 
    vector<vector<Cell>> maze(column, vector<Cell>(row));  //my maze
    string filename = "maze_" + to_string(mazeID) + ".txt";  //maze_ID.txt
    ifstream inFile;  // read maze.txt file to find a path
    inFile.open(filename);
    
    if (!inFile.is_open()) {
        cout << "Maze file cannot opened." << endl;
    }
    
    char ch;
    bool left, right, up , down;
    int x, y;
    
    string line;
    int line_count = 0;
    while(getline(inFile,line)){  //trying to read file line by line
        if(line_count > 0){
            
            istringstream strLine(line); //look string by string
            

            if(strLine >> ch >> ch >> x >> ch >> ch >> y >> ch >> ch >> left >> ch >> ch >> right >> ch >> ch >> up >> ch >> ch >> down){  // reading from maze text file
                Cell readcell;
                readcell.coorX = x;
                readcell.coorY = y;
                readcell.l= left;
                readcell.r = right;
                readcell.u = up;
                readcell.d = down;
                maze[x][y] = readcell;
                
            }
            
        }
        line_count++;
    }
    
    
    //maze_1_path_0_0_31_19 -> example path file name
    string filename_path = "maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
    
    ofstream pathFile;
    
    pathFile.open(filename_path);
    
    
    Stack<Cell> pathStack;  //start with empty stack
    
    //initialize the first cell and push it to stack
    Cell entryCell = maze[entryX][entryY];
    pathStack.push(entryCell);
    entryCell.visited = true;
    Cell targetCell;
    
    int size = 0;
    
    
    // find next cell algorithm in loop to go exit
    while(!pathStack.isEmpty()){
        bool found = false;
        Cell currCell = pathStack.top();  //at the beginnng entry cell
        int X = currCell.coorX;
        int Y = currCell.coorY;
        
        if(currCell.coorX == exitX && currCell.coorY == exitY){
            break;  //path found
            
        }
        currCell. visited = true;
        Cell nextCell;
        if(currCell.l == false && maze[X-1][Y].visited == false){
            found = true;
            nextCell = maze[X-1][Y];
        }
        if(currCell.r == false && maze[X+1][Y].visited == false){
            found = true;
            nextCell = maze[X+1][Y];
        }
        if(currCell.u == false && maze[X][Y+1].visited == false){
            found = true;
            nextCell = maze[X][Y+1];
        }
        if(currCell.d == false && maze[X][Y-1].visited == false){
            found = true;
            nextCell = maze[X][Y-1];
        }
        
        if(!found){  //if we cannot find cell to move
            nextCell = currCell;
        }
        
        if(nextCell.coorX == X && nextCell.coorY == Y){
            //  backtracking after we cannot go somewhere to go and popping top element from stack then go back in maze
            pathStack.pop();
            size--;
        }
        else{
            pathStack.push(nextCell);  //make nextcell top element
            size++;
            nextCell.visited = true; //Update
            maze[nextCell.coorX][nextCell.coorY].visited = true; //that location visited
        }
        
        
    }
    
    //
    //output PathFile
    //
    
    // I have correct pathStack in here but I need to find it reversed way for the path road
    Stack<Cell> TrackStack;
    
    pathFile << entryCell.coorX << " " << entryCell.coorY << endl;  // start with entry cells coors
    
    for(int i=0; i<size; i++){  //pop all elements
        Cell roadCell = pathStack.top();
        TrackStack.push(roadCell);  // push top element to other stack and then pop it for reversing the path way.
        pathStack.pop();
    }
    while(!TrackStack.isEmpty()){
        pathFile << TrackStack.top().coorX << " " << TrackStack.top().coorY << endl;
        TrackStack.pop();
        
    }
    
    inFile.close();
    pathFile.close();
    
}




int main() {
    
    int numberofMazes, row, column, mazeId;
    int entryX, entryY;
    int exitX, exitY;
    
    cout << "Enter the number of mazes: ";
    cin >> numberofMazes;
    
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> row >> column;
    
    for (int i = 1;i<=numberofMazes;i++){
        MazeGenerate(row, column, i);  // mazeIDs starts from 1
    }
    
    
    cout << "All mazes are generated." << endl;
    cout << endl;
    
    cout << "Enter a maze ID between 1 to " << numberofMazes << " inclusive to find a path: ";
    cin >> mazeId;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryX >> entryY;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;
    
    PathFinding(row, column, mazeId, entryX, entryY, exitX, exitY);
     
    return 0;
}

