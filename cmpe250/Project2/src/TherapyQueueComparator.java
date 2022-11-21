import java.util.Comparator;

/**
 * Comparator of TherapyQueue.
 * Those who have higher training time have priority.
 * If two player has the same training time, then we check the arrival time.
 * The one arrived first has the priority.
 * If it is same, too, the one with smaller id is prioritized.
 * 
 * @author Mehmet
 *
 */
public class TherapyQueueComparator implements Comparator<Player> {

	@Override
	public int compare(Player p1, Player p2) {
		if (p1.getCurrentTrainTime() > p2.getCurrentTrainTime())
			return -1;
		else if (p1.getCurrentTrainTime() == p2.getCurrentTrainTime()) {
			if (p1.getArriveTime() < p2.getArriveTime())
				return -1;
			else if (p1.getArriveTime() == p2.getArriveTime())
				return (p1.getId() < p2.getId())?-1:1;
			else
				return 1;
		}
		else
			return 1;
	}
}
