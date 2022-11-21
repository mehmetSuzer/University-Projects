

/**
 * Therapist class which have the field of service time of therapists
 * If a therapist is not available, then he gives a training service to a player.
 * 
 * @author Mehmet
 */
public class Therapist {

	private double serviceTime;
	public boolean available;
	
	public Therapist(double serviceTime) {
		this.serviceTime = serviceTime;
		this.available = true;
	}
	
	/* -------- Getter and Setters -------- */

	public double getServiceTime() {
		return serviceTime;
	}

	public void setServiceTime(double serviceTime) {
		this.serviceTime = serviceTime;
	}
	
}
