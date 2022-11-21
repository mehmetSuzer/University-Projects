


import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Scanner;


public class project3main {
	/**
	 * Reads the input file and runs the simulation.
	 * Prints the shortest path from mecnunCity to leylaCity and the cost of min spanning tree into the output file.
	 * 
	 * @param args
	 * @throws FileNotFoundException
	 */
	public static void main(String[] args) throws FileNotFoundException {
		// Open the input and output file
		Scanner in = new Scanner(new File(args[0]));
		PrintStream out = new PrintStream(new File(args[1]));
		
		int threshold = in.nextInt();
		int cityNumber = in.nextInt();
		ArrayList<City> cities = new ArrayList<City>(cityNumber);
		String mecnunCityId = in.next();
		String leylaCityId = in.next();
		in.nextLine();
		
		int cCityNumber = 0;
		int dCityNumber = 0;
		ArrayList<String[]> inputs = new ArrayList<String[]>(cityNumber);
		for (int i = 0; i < cityNumber; i++) {
			String[] words = in.nextLine().split(" ");
			inputs.add(words);
			cities.add(new City(words[0]));
			if (words[0].charAt(0) == 'd' && dCityNumber == 0) {
				cCityNumber = i;
				dCityNumber = cityNumber-cCityNumber;
			}
		}
		
		City mecnunCity = cities.get(Integer.parseInt(mecnunCityId.substring(1))-1);
		City leylaCity = cities.get(Integer.parseInt(leylaCityId.substring(1))-1);
		
		// Add roads to the cities
		for (int i = 0; i < cityNumber; i++) {
			String[] words = inputs.get(i);
			City city = cities.get(i);
			for (int j = 0; j < (words.length-1)/2; j++) {
				int index = Integer.parseInt(words[1+2*j].substring(1))-1;
				if (words[1+2*j].charAt(0) == 'd') 
					index += cCityNumber;
				City neighbor = cities.get(index);
				int weight = Integer.parseInt(words[2+2*j]);
				if (city.getType() == 'c' && neighbor.getType() == 'c')
					city.addRoad(neighbor, weight, true);
				else
					city.addRoad(neighbor, weight, false);
			}
		}
		
		Path shortestPath = Algorithms.shortestPath(cities, cCityNumber, mecnunCity, leylaCity);
		if (shortestPath != null) {
			// Print city ids
			for (City city: shortestPath.cities)
				out.print(city.getId() + " ");
			out.println();
			
			int totalWeight = -1;
			if (shortestPath.totalWeight <= threshold)
				totalWeight = Algorithms.minSpanningTree(cities, leylaCity);
			out.println(totalWeight);
		}
		else
			out.println(-1 + "\n" + -1);
		
		in.close();
		out.close();
	}
}







