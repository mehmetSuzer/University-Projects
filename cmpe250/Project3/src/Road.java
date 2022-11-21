

/**
 * Road class represents the edges between cities. 
 * Every city has a road arrayList.
 * 
 * @author Mehmet
 *
 */
public class Road {
	private City neighbor;
	private int weight;
	
	public Road(City neighbor, int weight) {
		this.neighbor = neighbor;
		this.weight = weight;
	}
	
	/*--------- Getter and Setters ----------*/

	public City getNeighbor() {
		return neighbor;
	}

	public void setNeighbor(City neighbor) {
		this.neighbor = neighbor;
	}

	public int getWeight() {
		return weight;
	}

	public void setWeight(int weight) {
		this.weight = weight;
	}
}
