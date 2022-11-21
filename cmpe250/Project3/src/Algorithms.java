


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.PriorityQueue;

public class Algorithms {

	/**
	 * Finds the shortest paths from mecnunCity to every other c cities using Dijkstra's Algorithm.
	 * Returns only the shortest path to leylaCity and the total length.
	 * 
	 * @param cities
	 * @param cCityNumber
	 * @param mecnunCity
	 * @param leylaCity
	 * @return The shortest path from mecnunCity to leylaCity and the total length of it
	 */
	public static Path shortestPath(ArrayList<City> cities, int cCityNumber, City mecnunCity, City leylaCity) {
		//Initialize the variables
		HashMap<City, City> predecessors = new HashMap<City, City>();
		HashMap<City, Integer> distances = new HashMap<City, Integer>();
		ArrayList<City> currentCities = new ArrayList<City>(); // cities whose distances are updated but which are not processed by dijkstra
		for (int i = 0; i < cCityNumber; i++) {
			City city = cities.get(i);
			predecessors.put(city, null);
			distances.put(city, Integer.MAX_VALUE);
		}
		distances.put(mecnunCity, 0);
		currentCities.add(mecnunCity);
		
		while (!currentCities.isEmpty()) {
			// Look for the closest city to mecnunCity in unprocessed
			int minDistanceIndex = 0;
			for (int i = 1; i < currentCities.size(); i++) {
				if (distances.get(currentCities.get(i)) < distances.get(currentCities.get(minDistanceIndex)))
					minDistanceIndex = i;
			}
			City city = currentCities.remove(minDistanceIndex); // closest city is removed from the unprocessed
			if (distances.get(city) == Integer.MAX_VALUE) // No city has road to this city, it is unreachable
				continue;
			
			for (Road road: city.getRoads()) { // check neighbors
				City neighbor = road.getNeighbor();
				// Skip the neighbors on the other side of the river
				if (neighbor.getType() == 'd')
					continue;
				
				int newDistance = distances.get(city) + road.getWeight();
				if (newDistance < distances.get(neighbor)) {
					if (distances.get(neighbor) == Integer.MAX_VALUE) // if the distance of that city is MAX_VALUE,
						currentCities.add(neighbor);				  // then it has neither fixed nor added to the currCities yet.
					distances.put(neighbor, newDistance);
					predecessors.put(neighbor, city);
				}
			}
		} // end of main loop
		if (distances.get(leylaCity) == Integer.MAX_VALUE) // No path to Leyla's City
			return null;
		
		// Follow the predecessors starting from leylaCity and find the shortest path
		// Return the path with the total weight to leylaCity
		ArrayList<City> citiesOnPath = new ArrayList<City>();
		City currentCity = leylaCity;
		while (currentCity != null) {
			citiesOnPath.add(0, currentCity);
			currentCity = predecessors.get(currentCity);
		}
		Path path = new Path(citiesOnPath, distances.get(leylaCity));
		return path;
	}
	
	/**
	 * Find the minimum spanning tree cost using Prim's Algorithm.
	 * 
	 * @param cities
	 * @param leylaCity
	 * @return the min total cost of the honeymoon which is the cost of min spanning tree times 2
	 */
	public static int minSpanningTree(ArrayList<City> cities, City leylaCity) {
		HashMap<City, Integer> costs = new HashMap<City, Integer>();
		HashMap<City, Boolean> known = new HashMap<City, Boolean>();
		for (City city: cities) {
			if (city.getType() == 'd' || city.equals(leylaCity)) {
				costs.put(city, Integer.MAX_VALUE);
				known.put(city, false);
			}
		}
		costs.put(leylaCity, 0);
		PriorityQueue<Pair> queue = new PriorityQueue<Pair>();
		queue.add(new Pair(leylaCity, 0));
		
		while (!queue.isEmpty()) {
			Pair pair = queue.poll();
			if (!known.get(pair.city)) {
				known.put(pair.city, true);
				for (Road road: pair.city.getRoads()) {
					City neighbor = road.getNeighbor();
					// If the neighbor is a c city which is not leylaCity or it is known and added to the tree, then go to the next neighbor
					if ((neighbor.getType() == 'c' && !neighbor.equals(leylaCity)) || known.get(neighbor))
						continue;
					
					if (road.getWeight() < costs.get(neighbor)) {
						costs.put(neighbor, road.getWeight());
						Pair newPair = new Pair(neighbor, costs.get(neighbor));
						if (!queue.contains(newPair))
							queue.add(newPair);	
						else {
							// Find the pair and replace it with the new one
							Iterator<Pair> iter = queue.iterator();
							while (iter.hasNext()) {
								if (iter.next().city.equals(neighbor))
									break;
							}
							iter.remove();
							queue.add(newPair);
						}
					}
				}
			}
		} // end of main loop
		// Check whether all cities are traveled or not
		boolean allCitiesAreTraveled = true;
		for (City city: known.keySet())
			allCitiesAreTraveled = allCitiesAreTraveled && known.get(city);
		
		if (allCitiesAreTraveled) {
			int result = 0;
			for (City city: costs.keySet())
				result += costs.get(city);
			return 2*result;
		}
		else
			return -2;
	}
}






