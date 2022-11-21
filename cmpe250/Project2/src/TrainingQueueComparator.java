
import java.util.Comparator;

/**
 * Comparator of TrainingQueue.
 * The one arrived first has the priority.
 * If they arrive at the same time, the player with smaller id is prioritized.
 * 
 * @author Mehmet
 *
 */
class TrainingQueueComparator implements Comparator<Player> {

	@Override
	public int compare(Player p1, Player p2) {
		if (p1.getArriveTime() < p2.getArriveTime())
			return -1;
		else if (p1.getArriveTime() == p2.getArriveTime())
			return (p1.getId() < p2.getId())?-1:1;
		else
			return 1;
		
	}

}
