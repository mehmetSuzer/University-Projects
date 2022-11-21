

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.Scanner;

public class project5main {
	public static void main(String[] args) throws FileNotFoundException {
		Scanner in = new Scanner(new File(args[0]));
		PrintStream out = new PrintStream(new File(args[1]));
		
		String typeLine = in.nextLine();
		String[] types = typeLine.split(" ");
		int productNum = types.length;
		
		int[] ATimes = new int[productNum];
		for (int i = 0; i < productNum; i++)
			ATimes[i] = in.nextInt();
		int[] BTimes = new int[productNum];
		for (int i = 0; i < productNum; i++)
			BTimes[i] = in.nextInt();
		int[] profits = new int[productNum];
		for (int i = 0; i < productNum; i++)
			profits[i] = in.nextInt();
		int[] startTimes = new int[productNum];
		for (int i = 0; i < productNum; i++)
			startTimes[i] = in.nextInt();
		
		Product[] offers = new Product[productNum];
		for (int i = 0; i < productNum; i++)
			offers[i] = new Product(types[i], ATimes[i], BTimes[i], profits[i], startTimes[i]);
		quickSort(offers, 0, offers.length-1);
		int[] maxProfit = new int[productNum+1];
		
		for (int i = 1; i <= productNum; i++) {
			int j = i-1;
			while (j >= 1 && offers[j-1].endTime > offers[i-1].startTime)
				j--;
			maxProfit[i] = max(maxProfit[i-1], maxProfit[j] + offers[i-1].profit);
		}
		int totalProfit = maxProfit[productNum];
		out.print(totalProfit);
		
		in.close();
		out.close();
	}
	
	/**
	 * Swaps two elements in an array at the specified indices.
	 * 
	 * @param <E>
	 * @param arr
	 * @param index1
	 * @param index2
	 */
	public static <E> void swap(E[] arr, int index1, int index2) {
		E temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}
	
	/**
	 * @param <E>
	 * @param first
	 * @param second
	 * @return smaller parameter
	 */
	public static <E extends Comparable<E>> E min(E first, E second) {
		return (first.compareTo(second) < 0)?first:second;
	}
	
	/**
	 * @param <E>
	 * @param first
	 * @param second
	 * @return greater parameter
	 */
	public static <E extends Comparable<E>> E max(E first, E second) {
		return (first.compareTo(second) > 0)?first:second;
	}
	
	/**
	 * Sorts an array using the quickSort algorithm.
	 * 
	 * @param <E>
	 * @param arr
	 * @param low index
	 * @param high index
	 */
	public static <E extends Comparable<E>> void quickSort(E[] arr, int low, int high) {
		if (low >= high)
			return;
		
		int middle = low + (high-low)/2;
		// median of three: first, middle, and last
		E pivot = min(min(max(arr[low], arr[middle]), max(arr[middle], arr[high])), max(arr[low], arr[high]));
		int lowIndex = low, highIndex = high;
		while (lowIndex <= highIndex) {
			while (arr[lowIndex].compareTo(pivot) < 0)
				lowIndex++;
			while (arr[highIndex].compareTo(pivot) > 0)
				highIndex--;
			if (lowIndex <= highIndex)
				swap(arr, lowIndex++, highIndex--);
		}
		quickSort(arr, low, highIndex);
		quickSort(arr, lowIndex, high);
	}
}




