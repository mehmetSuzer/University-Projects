


import java.util.ArrayList;

/**
 * Path class is implemented to return both cities on the path and the total length from the Dijkstra's algorithm.
 * @author Mehmet
 *
 */
public class Path {
	public ArrayList<City> cities;
	public int totalWeight;
	
	public Path(ArrayList<City> cities, int totalWeight) {
		this.cities = cities;
		this.totalWeight = totalWeight;
	}
}
