


import java.util.ArrayList;

/**
 * City class represents the cities in the map.
 * If a city is in the left side of the river, its type is c.
 * If it in in the right, then its type is d.
 * Every city has an arrayList which contains its neighbors and weights of the edges between them.
 * 
 * @author Mehmet
 *
 */
public class City {
	private String id;
	private ArrayList<Road> roads;
	private char type;
	
	public City(String id) {
		this.id = id;
		roads = new ArrayList<Road>();
		this.type = id.charAt(0);
	}
	
	/**
	 * Adds an edge with the given weight between this city and the given neighbor city.
	 * If the edge is wanted to be undirected, then the edge is added to the neighbor, too.
	 * 
	 * @param neighbor
	 * @param weight
	 * @param directed
	 */
	public void addRoad(City neighbor, int weight, boolean directed) {
		Road road = new Road(neighbor, weight);
		roads.add(road);
		if (!directed)
			neighbor.addRoad(this, weight, true);
	}
	
	/**
	 * Returns the neighbors.
	 * @return all the neighbors of the city
	 */
	public ArrayList<City> getNeighbors() {
		ArrayList<City> result = new ArrayList<City>();
		for (Road road: roads)
			result.add(road.getNeighbor());
		return result;
	}
	
	/**
	 * If two city has the same id, then they are considered to be equal.
	 * @param other
	 */
	public boolean equals(City other) {
		return this.id == other.id;
	}
	
	public String toString() {
		return id;
	}
	
	/*--------- Getter and Setters ----------*/

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public ArrayList<Road> getRoads() {
		return roads;
	}

	public void setRoads(ArrayList<Road> roads) {
		this.roads = roads;
	}

	public char getType() {
		return type;
	}

	public void setType(char type) {
		this.type = type;
	}
}





