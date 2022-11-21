
/**
 * Student class implementing comparable interface.
 * 
 * id: a student specific number.
 * name: name of a student.
 * duration: the number of semesters for which a student want to rent a house.
 * rating: the minimum rating for a student to rent a house.
 * 
 * @author Mehmet
 */
public class Student implements Comparable<Student> {
	private int id;
	private String name;
	private int duration;
	private double rating;
	
	public Student(int id, String name, int duration, double rating) {
		this.id = id;
		this.name = name;
		this.duration = duration;
		this.rating = rating;
	}

	/**
	 * Students which have higher ids are greater than those which have lower ids
	 */
	@Override
	public int compareTo(Student o) {
		if (id > o.id)
			return 1;
		else if (id < o.id)
			return -1;
		return 0;
	}
	
	/*--------- Getters and Setters ------------*/

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
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
