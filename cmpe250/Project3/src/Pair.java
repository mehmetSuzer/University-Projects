
/**
 * Pair class is implemented to add the cities with their costs into a priority queue easily.
 * 
 * @author Mehmet
 *
 */
public class Pair implements Comparable<Pair> {
	public City city;
	public int weight;
	
	public Pair(City city, int weight) {
		this.city = city;
		this.weight = weight;
	}

	/**
	 * If a pair has lower weight, then it is closer to the top of the priority queue.
	 */
	@Override
	public int compareTo(Pair other) {
		if (this.weight < other.weight)
			return -1;
		else if (this.weight == other.weight)
			return 0;
		else
			return 1;
	}
	
	/**
	 * If two pair have the same city, then they are equal regardless of their weights.
	 * @param other
	 */
	public boolean equals(Pair other) {
		return this.city == other.city;
	}
}
