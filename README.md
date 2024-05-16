# Graph Traversal Sequencer
* Desktop application for drawing graphs and generating midi note sequences
* Graph nodes to represent midi notes and edges to represent connections between midi notes 
* Generates all possible sequences from exploring all possible paths through graph from user specified start node


### Follow link to download for Windows
https://app.box.com/s/xfcio6glciu63e0uqhwhqmjvi1tb092r
</br>

Draw nodes of midi notes and connecting edges to create music sequences</br>

![alt_image](https://github.com/nathanfenoglio/Graph-Traversal-Sequencer/blob/master/readme_images/GraphTraversalSequencerImage.jpg)
### How To Use
![alt_image](https://github.com/nathanfenoglio/Graph-Traversal-Sequencer/blob/master/readme_images/GraphTraversalSequencerInstructionsImage.jpg)

GRAPH BUILDING OPTIONS to select </br>
  NO DRAW - no node or edge is added/removed </br>
  ADD NODE - place node at position of mouse click on screen</br> 
  ADD EDGE - click "originating node" of edge that you wish to add from graph, then click "end of arrow" node to add edge from "originating node" to "end of arrow" node</br>
  REMOVE NODE - remove node at position of mouse click and all edges connected</br>
  REMOVE EDGE - click "originating node" of edge that you wish to remove from graph, then click "end of arrow" node to remove edge
  
DIRECTED/UNDIRECTED GRAPH - </br>select undirected if you want to add an edge in both directions when adding an edge

START NODE - </br>specify the start node (index of node to begin traversing graph from)

Can specify a midi note for each individual node or defaults to node index number</br>
\tFor instance music note middle C is midi note # 60

GENERATE SEQUENCE - </br>
Finds all possible paths through graph. Lists each path separately and then concatenates into ALL POSSIBLE PATHS ALTOGETHER box to copy

\# of notes in all possible paths altogether - </br>
Displays # of notes when putting together all of the individual possible graph traversal sequences

factors of # of notes - </br>
Just displaying the factors of the # of notes in the ALL POSSIBLE PATHS ALTOGETHER sequence for reference

Adjacency Matrix - </br> 
Display matrix representation of graph. For each row, the column # represents a node able to be traveled to from the row node
