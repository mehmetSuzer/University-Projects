

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;

/**
 * Graph implemented for max flow problem with an adjacency list.
 * @author Mehmet
 */
public class Graph {
	public int vertexNum;
	//			     vertex         neighbor  weight
	private HashMap<Integer, HashMap<Integer, Integer>> edges;
	private HashMap<Integer, Integer> levels;
	private int source; // first vertex
	private int sink; // last vertex
	
	public Graph(int vertexNum) {
		this.vertexNum = vertexNum;
		this.edges = new HashMap<Integer, HashMap<Integer, Integer>>(2*vertexNum);
		this.levels = new HashMap<Integer ,Integer>(2*vertexNum);
		for (int vertex = 0; vertex < vertexNum; vertex++) {
			edges.put(vertex, new HashMap<Integer, Integer>());
			levels.put(vertex, -1);
		}
		this.source = 0;
		this.sink = vertexNum-1;
	}
	
	/**
	 * @param vertex
	 * @return a set of neighbors of the vertex
	 */
	public Set<Integer> getNeighbors(int vertex) {
		return edges.get(vertex).keySet();
	}
	
	/**
	 * @param vertex
	 * @return an array of neighbors of the vertex
	 */
	public int[] getNeighborArray(int vertex) {
		Set<Integer> neighbors = this.getNeighbors(vertex);
		int[] result = new int[neighbors.size()];
		int i = 0;
		for (int neighbor: neighbors)
			result[i++] = neighbor;
		return result;
	}
	
	/**
	 * If there is already edge between first and second vertices, it increases the edge weight.
	 * Otherwise, an edge between vertices with the specified weight is created .
	 * If edge weight is zero, then it does nothing.
	 * 
	 * @param first
	 * @param second
	 * @param weight
	 */
	public void addEdge(int first, int second, int weight) {
		if (weight == 0)
			return;
		if (this.getNeighbors(first).contains(second))
			this.increaseEdgeWeight(first, second, weight);
		else 
			this.edges.get(first).put(second, weight);
	}
	
	/**
	 * Increases the edge weight between vertices given.
	 * If the final edge weight is 0, then the edge is removed.
	 * 
	 * @param first
	 * @param second
	 * @param amount
	 */
	public void increaseEdgeWeight(int first, int second, int amount) {
		int initialWeight = this.edges.get(first).get(second);
		if (initialWeight+amount == 0)
			this.removeEdge(first, second);
		else
			this.edges.get(first).put(second, initialWeight+amount);
	}
	
	/**
	 * Removes the edge from the first vertex to the second vertex.
	 * 
	 * @param first
	 * @param second
	 */
	public void removeEdge(int first, int second) {
		this.edges.get(first).remove(second);
	}
	
	/**
	 * Runs breadth-first search algorithm to assign levels to nodes.
	 * 
	 * @return whether the sink is reachable from the source
	 */
	public boolean bfs() {
		for (int i = 0; i < vertexNum; i++)
			levels.put(i, -1);
		levels.put(source, 0);
		
		Queue<Integer> queue = new LinkedList<Integer>();
		queue.add(source);
		
		while (!queue.isEmpty()) {
			int vertex = queue.poll();
			for (int neighbor: this.getNeighbors(vertex)) {
				if (levels.get(neighbor) == -1) { // if the vertex is not processed
					levels.put(neighbor, levels.get(vertex)+1);
					queue.add(neighbor);
				}
			}
		}
		return (levels.get(sink)== -1)?false:true; // return true if there is a path to the sink
	}
	
	/**
	 * Sends flows recursively to the sink.
	 * Every vertex in the path is in an increasing level order.
	 * If a flow is sent, then edges on the graph is updated.
	 * 
	 * @param currVertex
	 * @param minFlow
	 * @param indices
	 * @return flow of the path
	 */
	public int sendFlow(int currVertex, int minFlow, int[] indices) {
		// Base case
		if (currVertex == sink)
			return minFlow;
		
		// Recursive case
		int[] neighborArray = this.getNeighborArray(currVertex);
		while (indices[currVertex] < neighborArray.length) {
			int neighbor = neighborArray[indices[currVertex]];
			if (levels.get(neighbor) == levels.get(currVertex)+1) { // pick a neighbor at the next level
				int edgeWeight = edges.get(currVertex).get(neighbor);
				int currFlow = (edgeWeight < minFlow)?edgeWeight:minFlow;
				int totalFlow = this.sendFlow(neighbor, currFlow, indices);
				if (totalFlow != 0) {
					this.increaseEdgeWeight(currVertex, neighbor, -totalFlow); // decrease edge weight by totalFlow
					this.addEdge(neighbor, currVertex, totalFlow); // add a reverse edge with flow of totalFlow
					return totalFlow;
				}
			}
			indices[currVertex]++;
		}
		return 0; // can't reach sink
	}
}





