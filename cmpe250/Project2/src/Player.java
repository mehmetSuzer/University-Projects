


/**
 * Player class which contains fields of id, skill level, and many statistical data of players.
 * During the simulation, statistical data of the players are kept in these fields.
 * Players have different orderings in different priority queues ( train, therapy, massage queues).
 * If a player is not available, meaning that she is in a queue or getting a service, she does not respond to events.
 * 
 * @author Mehmet
 *
 */
public class Player {

	private int id;
	private int skillLevel;
	private int trainAttempt;  // which is equal to the therapy attempt
	private int massageAttempt; // max value is 3
	
	private double currentTrainTime;
	private double currentMassageTime;
	
	private double trainQueueEnterTime;
	private double therapyQueueEnterTime;
	private double massageQueueEnterTime;
	
	private double arriveTime; // It is used to sort the players in queues
	
	private double totalTrainQueueTime;
	private double totalTherapyQueueTime;
	private double totalMassageQueueTime;
	
	private double totalTrainTime;
	private double totalTherapyTime;
	private double totalMassageTime;
	private double totalTurnAroundTime;
	
	public boolean available;
	
	public Player(int id, int skillLevel) {
		this.id = id;
		this.skillLevel = skillLevel;
		this.trainAttempt = 0;
		this.massageAttempt = 0;
		
		this.currentTrainTime = 0;
		this.currentMassageTime = 0;
		
		this.trainQueueEnterTime = 0;
		this.therapyQueueEnterTime = 0;
		this.massageQueueEnterTime = 0;
		
		this.arriveTime = 0;

		this.totalTrainQueueTime = 0;
		this.totalTherapyQueueTime = 0;
		this.totalMassageQueueTime = 0;
		
		this.totalTrainTime = 0;
		this.totalTherapyTime = 0;
		this.totalMassageTime = 0;
		this.totalTurnAroundTime = 0;
		
		this.available = true;
	}
	
	/**
	 * Increases the total train queue waiting time and training attempt. 
	 * @param time
	 */
	public void updateTotalTrainQueueTime(double time) {
		this.totalTrainQueueTime += time-this.trainQueueEnterTime;
		this.trainAttempt++;
	}
	
	/**
	 * Increases the total physiotherapy queue waiting time 
	 * @param time
	 */
	public void updateTotalTherapyQueueTime(double time) {
		this.totalTherapyQueueTime += time-this.therapyQueueEnterTime;
	}
	
	/**
	 * Increases the total massage queue waiting time 
	 * @param time
	 */
	public void updateTotalMassageQueueTime(double time) {
		this.totalMassageQueueTime += time-this.massageQueueEnterTime;
	}
	
	/**
	 * Adds current train duration to the total duration
	 */
	public void updateTotalTrainTime() {
		this.totalTrainTime += this.currentTrainTime;
	}
	
	/**
	 * Adds service time of the physiotherapist to total therapy time of the player
	 * @param serviceTime
	 */
	public void updateTotalTherapyTime(double serviceTime) {
		this.totalTherapyTime += serviceTime;
	}
	
	/**
	 * Adds current massage duration to the total duration
	 */
	public void updateTotalMassageTime() {
		this.totalMassageTime += this.currentMassageTime;
	}
	
	/**
	 * Adds current turnaround duration to the total duration
	 * @param time
	 */
	public void updateTotalTurnAroundTime(double time) {
		this.totalTurnAroundTime += time-this.trainQueueEnterTime;
	}
	
	/**
	 * Increments massage attempt by 1
	 */
	public void incrementMassageAttempt() {
		this.massageAttempt++;
	}
	
	/**
	 * If two players have same id, them they are equal.
	 * Otherwise, they are not.
	 */
	public boolean equals(Object other) {
		if (!(other instanceof Player))
			return false;
		Player player = (Player) other;
		return (this.id == player.id)?true:false;
	}
	
	/* ---------- Getter and Setters ----------- */

	public int getId() {
		return id;
	}

	public int getSkillLevel() {
		return skillLevel;
	}

	public int getMassageAttempt() {
		return massageAttempt;
	}

	public int getTrainAttempt() {
		return trainAttempt;
	}

	public double getCurrentTrainTime() {
		return currentTrainTime;
	}

	public void setCurrentTrainTime(double currentTrainTime) {
		this.currentTrainTime = currentTrainTime;
	}

	public double getCurrentMassageTime() {
		return currentMassageTime;
	}

	public void setCurrentMassageTime(double currentMassageTime) {
		this.currentMassageTime = currentMassageTime;
	}

	public double getArriveTime() {
		return arriveTime;
	}

	public void setArriveTime(double arriveTime) {
		this.arriveTime = arriveTime;
	}

	public double getTotalTrainQueueTime() {
		return totalTrainQueueTime;
	}

	public double getTotalTherapyQueueTime() {
		return totalTherapyQueueTime;
	}

	public double getTotalMassageQueueTime() {
		return totalMassageQueueTime;
	}

	public double getTrainQueueEnterTime() {
		return trainQueueEnterTime;
	}

	public void setTrainQueueEnterTime(double trainQueueEnterTime) {
		this.trainQueueEnterTime = trainQueueEnterTime;
	}

	public double getMassageQueueEnterTime() {
		return massageQueueEnterTime;
	}

	public void setMassageQueueEnterTime(double massageQueueEnterTime) {
		this.massageQueueEnterTime = massageQueueEnterTime;
	}

	public double getTherapyQueueEnterTime() {
		return therapyQueueEnterTime;
	}

	public void setTherapyQueueEnterTime(double therapyQueueEnterTime) {
		this.therapyQueueEnterTime = therapyQueueEnterTime;
	}

	public double getTotalTrainTime() {
		return totalTrainTime;
	}

	public double getTotalTherapyTime() {
		return totalTherapyTime;
	}

	public double getTotalMassageTime() {
		return totalMassageTime;
	}

	public double getTotalTurnAroundTime() {
		return totalTurnAroundTime;
	}
}







