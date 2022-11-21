

public class Product implements Comparable<Product> {
	public int profit;
	public int startTime;
	public int endTime;
	
	public Product(String type, int ATime, int BTime, int profit, int startTime) {
		this.profit = profit;
		this.startTime = startTime;
		char ch = type.charAt(0);
		this.endTime = this.startTime + ((ch=='s')?ATime:BTime);
	}

	/**
	 * If the end time is greater than the other's end time, returns 1.
	 * If it is less, returns -1.
	 * Otherwise returns 0.
	 */
	@Override
	public int compareTo(Product other) {
		if (this.endTime > other.endTime)
			return 1;
		else if (this.endTime < other.endTime)
			return -1;
		return 0;
	}
}
