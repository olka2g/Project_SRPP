#include "Dijkstra.h"

Route DijkstraOptimization(Route r)
{
	// TODO:
	/*
	
	2      dist[source]  := 0                     // Distance from source to source
	3      for each vertex v in Graph:            // Initializations
	4          if v ≠ source
	5              dist[v]  := infinity           // Unknown distance function from source to v
	6              previous[v]  := undefined      // Previous node in optimal path from source
	7          end if 
	8          add v to Q                         // All nodes initially in Q (unvisited nodes)
	9      end for
	10      
	11      while Q is not empty:                  // The main loop
	12          u := vertex in Q with min dist[u]  // Source node in first case
	13          remove u from Q 
	14          
	15          for each neighbor v of u:           // where v has not yet been removed from Q.
	16              alt := dist[u] + length(u, v)
	17              if alt < dist[v]:               // A shorter path to v has been found
	18                  dist[v]  := alt 
	19                  previous[v]  := u 
	20              end if
	21          end for
	22      end while
	23      return dist[], previous[]
	
	
	*/

	return r;
}