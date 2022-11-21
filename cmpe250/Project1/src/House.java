

/**
 * House class implementing Comparable interface
 * 
 * id: the house specific number.
 * duration: the number of semesters a house is rented for.
 * rating: the number specifying the quality of a house.
 * 
 * @author Mehmet
 */
public class House implements Comparable<House> {
	private int id;
	private int duration;
	private double rating;
	
	public House(int id, int duration, double rating) {
		this.id = id;
		this.duration = duration;
		this.rating = rating;
	}

	/**
	 * Houses which have higher ids are greater than those which have lower ids
	 */
	@Override
	public int compareTo(House o) {
		if (id > o.id)
			return 1;
		else if (id < o.id)
			return -1;
		return 0;
	}
	
	/**
	 * Decreases the duration of rented houses
	 */
	public void goNextSemester() {
		if (this.duration > 0)
			this.duration -= 1;
	}
	

	/*--------- Getters and Setters ------------*/
	
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getDuration() {
		return duration;
	}

	public void setDuration(int duration) {
		this.duration = duration;
	}

	public double getRating() {
		return rating;
	}

	public void setRating(double rating) {
		this.rating = rating;
	}
}
