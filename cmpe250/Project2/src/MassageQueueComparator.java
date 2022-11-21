import java.util.Comparator;

/**
 * Comparator of MassageQueue.
 * Those who have higher skill level have priority.
 * If two player has the same skill level, then we check the arrival time.
 * The one arrived first has the priority.
 * If it is same, too, the player with smaller id is prioritized.
 * 
 * @author Mehmet
 *
 */
public class MassageQueueComparator implements Comparator<Player> {

	@Override
	public int compare(Player p1, Player p2) {
		if (p1.getSkillLevel() > p2.getSkillLevel())
			return -1;
		else if (p1.getSkillLevel() == p2.getSkillLevel()) {
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
