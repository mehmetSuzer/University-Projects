

/**
 * Events in the simulation are reduced to 5 basic events.
 * 1- Enter Training Queue
 * 2- Exit Training and Enter Physiotherapy Queue
 * 3- Exit Physiotherapy
 * 4- Enter Massage Queue
 * 5- Exit Massage
 * 
 * Events are comparable on the basis of their occurrence time.
 * 
 * @author Mehmet
 *
 */
public class Event  implements Comparable<Event> {
	private String type;
	private int playerID;
	private double occurrenceTime;
	private double duration;
	private int staffId;
	
	public static final String ENTER_TQ = "t";
	public static final String EXIT_T_ENTER_PQ = "epq";
	public static final String EXIT_P = "ep";
	public static final String ENTER_MQ = "m";
	public static final String EXIT_M = "em";
	
	public static final int UNIMPORTANT_STAFF_ID = -1;
	public static final double UNIMPORTANT_DURATION = 0;
	
	public Event(String type, int playerID, double occurrenceTime, double duration, int staffId) {
		this.type = type;
		this.playerID = playerID;
		this.occurrenceTime = occurrenceTime; // event occurs at this time
		this.duration = duration; // if the duration is not important, it is set to 0
		this.staffId = staffId; // -1 if the event is related to massage or training, otherwise it is the therapist's index
	}
	
	/**
	 * Those which occur earlier are smaller and close to the head of the priority queue.
	 * @param other
	 */
	@Override
	public int compareTo(Event other) {
		if (this.occurrenceTime < other.occurrenceTime)
			return -1;
		else if (this.occurrenceTime > other.occurrenceTime)
			return 1;
		return 0;
	}

	/* ---------- Getter and Setters ----------- */
	
	public String getType() {
		return type;
	}

	public int getPlayerID() {
		return playerID;
	}

	public double getOccurrenceTime() {
		return occurrenceTime;
	}

	public double getDuration() {
		return duration;
	}

	public int getStaffId() {
		return staffId;
	}
}






