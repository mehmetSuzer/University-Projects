


import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;



/**
 * Simulation of players who take training, physiotherapy, and massage services.
 * 
 * @author Mehmet
 */
public class project2main {

	public static void main(String[] args) throws FileNotFoundException {
		// Open the input and output file
		Scanner in = new Scanner(new File(args[0]));
		PrintStream out = new PrintStream(new File(args[1]));
		
		// Comparators
		TrainingQueueComparator trainCom = new TrainingQueueComparator();
		TherapyQueueComparator therapyCom = new TherapyQueueComparator();
		MassageQueueComparator massageCom = new MassageQueueComparator();
		
		// Collections of players, events, and therapists
		ArrayList<Player> players = new ArrayList<Player>();
		PriorityQueue<Event> events = new PriorityQueue<Event>();
		ArrayList<Therapist> therapists = new ArrayList<Therapist>();
		
		// Number of available staffs
		int availableCoaches = 0;
		int availableTherapists = 0;
		int availableMasseurs = 0;
		
		// Statistical values
		double time = 0;
		int invalidAttemptNum = 0;
		int canceledAttemptNum = 0;
		int maxTrainQueueLength = 0;
		int maxTherapyQueueLength = 0;
		int maxMassageQueueLength = 0;
		
		// Service Queues
		PriorityQueue<Player> trainQueue = new PriorityQueue<Player>(trainCom); // a player with small id has the priority
		PriorityQueue<Player> therapyQueue = new PriorityQueue<Player>(therapyCom); // a player with higher training time has the priority
		PriorityQueue<Player> massageQueue = new PriorityQueue<Player>(massageCom); // a player with higher skill has the priority
		
		// Initialize players and store them in the arraylist
		int playerNum = in.nextInt();
		for (int i = 0; i < playerNum; i++) {
			int id = in.nextInt();
			int skillLevel = in.nextInt();
			players.add(new Player(id, skillLevel));
		}
		
		// Initialize events and store them in the pq sorted with respect to their occurrence time
		int initialEventNum = in.nextInt();
		for (int i = 0; i < initialEventNum; i++) {
			String type = in.next(); // m or t
			int playerID = in.nextInt();
			double arrivalTime = in.nextDouble();
			double duration = in.nextDouble();
			events.add(new Event(type, playerID, arrivalTime, duration, Event.UNIMPORTANT_STAFF_ID));
		}
		
		// Add therapists
		availableTherapists = in.nextInt();
		for (int i = 0; i < availableTherapists; i++) {
			double serviceTime = in.nextDouble();
			therapists.add(new Therapist(serviceTime));
		}
		
		// Add coaches
		availableCoaches = in.nextInt();
		
		// Add masseurs
		availableMasseurs = in.nextInt();
		
		while (!events.isEmpty()) {
			time = events.peek().getOccurrenceTime();
			Event event;
			Player player;
			
			// Process the events that occur at the same time
			while (!events.isEmpty() && time == events.peek().getOccurrenceTime()) {
				event = events.poll();
				player = players.get(event.getPlayerID());
				
				// If player is in train, physiotherapy, massage or in any queue, get the next event
				if (!player.available && (event.getType().equals(Event.ENTER_TQ) || event.getType().equals(Event.ENTER_MQ))) {
					if (event.getType().equals(Event.ENTER_MQ) && player.getMassageAttempt() == 3)
						invalidAttemptNum++;
					else
						canceledAttemptNum++;
					continue;
				}
				
				// Event occurs 
				if (event.getType().equals(Event.ENTER_TQ)) {
					player.setArriveTime(time);
					player.available = false;
					trainQueue.add(player);
					player.setTrainQueueEnterTime(time);
					player.setCurrentTrainTime(event.getDuration());
				}	
				else if (event.getType().equals(Event.EXIT_T_ENTER_PQ)) {
					availableCoaches++;
					player.setArriveTime(time);
					therapyQueue.add(player);
					player.setTherapyQueueEnterTime(time);
				}
				else if (event.getType().equals(Event.EXIT_P)) {
					player.updateTotalTurnAroundTime(time);
					therapists.get(event.getStaffId()).available = true;
					player.available = true;
					availableTherapists++;
				}
				else if (event.getType().equals(Event.ENTER_MQ)) {
					if (player.getMassageAttempt() == 3) {
						invalidAttemptNum++;
					}
					else {
						player.setArriveTime(time);
						player.available = false;
						massageQueue.add(player);
						player.setMassageQueueEnterTime(time);
						player.setCurrentMassageTime(event.getDuration());
						player.incrementMassageAttempt();
					}
				}
				else if (event.getType().equals(Event.EXIT_M)) {
					player.available = true;
					availableMasseurs++;
				}
			} // end of the inner loop
			
			// Check queues and make necessary allocations of players
			// If a player starts traning or getting physiotherapy or massage service, create exit event for that service
			
			// Training Queue
			if (!trainQueue.isEmpty() && availableCoaches != 0) {
				Player playerInQueue = trainQueue.poll();
				playerInQueue.updateTotalTrainQueueTime(time);
				playerInQueue.updateTotalTrainTime();
				availableCoaches--;
				Event newEvent = new Event(Event.EXIT_T_ENTER_PQ, playerInQueue.getId(), time+playerInQueue.getCurrentTrainTime(), Event.UNIMPORTANT_DURATION, Event.UNIMPORTANT_STAFF_ID);
				events.add(newEvent);
			}
			// Therapy Queue
			if (!therapyQueue.isEmpty() && availableTherapists != 0) {
				int therapistIndex = 0; 
				for (int i = 0; i < therapists.size(); i++) {
					if (therapists.get(i).available) {
						therapistIndex = i;
						break;
					}
				}
				Player playerInQueue = therapyQueue.poll();
				Therapist therapist = therapists.get(therapistIndex);
				therapist.available = false;
				availableTherapists--;
				playerInQueue.updateTotalTherapyQueueTime(time);
				playerInQueue.updateTotalTherapyTime(therapist.getServiceTime());
				Event newEvent = new Event(Event.EXIT_P, playerInQueue.getId(), time+therapist.getServiceTime(), Event.UNIMPORTANT_DURATION, therapistIndex);
				events.add(newEvent);
			}
			// Massage Queue
			if (!massageQueue.isEmpty() && availableMasseurs != 0) {
				Player playerInQueue = massageQueue.poll();
				playerInQueue.updateTotalMassageQueueTime(time);
				playerInQueue.updateTotalMassageTime();
				availableMasseurs--;
				Event newEvent = new Event(Event.EXIT_M, playerInQueue.getId(), time+playerInQueue.getCurrentMassageTime(), Event.UNIMPORTANT_DURATION, Event.UNIMPORTANT_STAFF_ID);
				events.add(newEvent);
			}
			
			// Update Max Queue Sizes
			int currSize = trainQueue.size();
			if (currSize > maxTrainQueueLength)
				maxTrainQueueLength = currSize;
			
			currSize = therapyQueue.size();
			if (currSize > maxTherapyQueueLength)
				maxTherapyQueueLength = currSize;
			
			currSize = massageQueue.size();
			if (currSize > maxMassageQueueLength)
				maxMassageQueueLength = currSize;
		} // end of the main loop
		
		// Calculate the average values
		double trainQueueTime = 0;
		double trainAttemptCount = 0;
		double averageTrainQueueTime = 0;
		
		double therapyQueueTime = 0;
		double averageTherapyQueueTime = 0;
		
		double massageQueueTime = 0;
		double massageAttemptCount = 0;
		double averageMassageQueueTime = 0;
		
		double trainTime = 0;
		double averageTrainTime = 0;
		double therapyTime = 0;
		double averageTherapyTime = 0;
		double massageTime = 0;
		double averageMassageTime = 0;
		double turnAroundTime = 0;
		double averageTurnAroundTime = 0;
		
		for (int i = 0; i < players.size(); i++) {
			trainQueueTime += players.get(i).getTotalTrainQueueTime();
			trainAttemptCount += players.get(i).getTrainAttempt();
			
			therapyQueueTime += players.get(i).getTotalTherapyQueueTime();
			
			massageQueueTime += players.get(i).getTotalMassageQueueTime();
			massageAttemptCount += players.get(i).getMassageAttempt();
			
			trainTime += players.get(i).getTotalTrainTime();
			therapyTime += players.get(i).getTotalTherapyTime();
			massageTime += players.get(i).getTotalMassageTime();
			turnAroundTime += players.get(i).getTotalTurnAroundTime();
		}
		
		if (trainAttemptCount != 0) {
			averageTrainQueueTime = trainQueueTime/trainAttemptCount;
			averageTherapyQueueTime = therapyQueueTime/trainAttemptCount;
			averageTrainTime = trainTime/trainAttemptCount;
			averageTherapyTime = therapyTime/trainAttemptCount;
			averageTurnAroundTime = turnAroundTime/trainAttemptCount;
		}
		if (massageAttemptCount != 0) {
			averageMassageQueueTime = massageQueueTime/massageAttemptCount;
			averageMassageTime = massageTime/massageAttemptCount;
		}
		
		Player playerSpentMostTimeInTherapyQueue = players.get(0);
		for (int i = 0; i < players.size(); i++) {
			Player player = players.get(i);
			if (player.getTotalTherapyQueueTime() > playerSpentMostTimeInTherapyQueue.getTotalTherapyQueueTime())
				playerSpentMostTimeInTherapyQueue = player;
		}
		
		int leastMassageQueueTimeIndex = -1;
		for (int i = 0; i < players.size(); i++) {
			Player player = players.get(i);
			if (player.getMassageAttempt() == 3) {
				if (leastMassageQueueTimeIndex == -1 || player.getTotalMassageQueueTime() < players.get(leastMassageQueueTimeIndex).getTotalMassageQueueTime())
					leastMassageQueueTimeIndex = player.getId();
			}
		}
		
		// Print statistical data into the output file
		out.printf("%d\n", maxTrainQueueLength);
		out.printf("%d\n", maxTherapyQueueLength);
		out.printf("%d\n", maxMassageQueueLength);
		out.printf("%.3f\n", averageTrainQueueTime);
		out.printf("%.3f\n", averageTherapyQueueTime);
		out.printf("%.3f\n", averageMassageQueueTime);
		out.printf("%.3f\n", averageTrainTime);
		out.printf("%.3f\n", averageTherapyTime);
		out.printf("%.3f\n", averageMassageTime);
		out.printf("%.3f\n", averageTurnAroundTime);
		out.printf("%d %.3f\n", playerSpentMostTimeInTherapyQueue.getId(), playerSpentMostTimeInTherapyQueue.getTotalTherapyQueueTime());
		out.printf("%d ", leastMassageQueueTimeIndex);
		if (leastMassageQueueTimeIndex == -1)
			out.printf("%d\n", -1);
		else
			out.printf("%.3f\n", players.get(leastMassageQueueTimeIndex).getTotalMassageQueueTime());
		out.printf("%d\n", invalidAttemptNum);
		out.printf("%d\n", canceledAttemptNum);
		out.printf("%.3f\n", time);
		
		in.close();
		out.close();
	}
}





