

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.Scanner;



public class project1main {

	/**
	 * Reads the input file which consisted of the information of students and houses,
	 * and creates necessary priority queues for these two types.
	 * Determines the maximum semester students study.
	 * 
	 * Every semester,it distributes students looking for a house to available houses, considering their rates,
	 * starting from the smallest indices for both houses and students. 
	 * 
	 * Students rented houses stay in these houses until they graduate.
	 * Finally, prints names of students who couldn't rent any house into a file in an id-descending order.
	 * 
	 * @param args
	 * @throws FileNotFoundException
	 */
	public static void main(String[] args) throws FileNotFoundException {
		
		// Open the input file
		// Open the output file, if it does not exist, create it
		Scanner in = new Scanner(new File(args[0]));
		PrintStream out = new PrintStream(new File(args[1]));
		
		// Priority Queues where students and houses stored with respect to their ids
		// Those which have smaller ids are closer to the head of the priority queue
		PriorityQueue<Student> students = new PriorityQueue<Student>();
		PriorityQueue<House> houses = new PriorityQueue<House>();
		
		String variableType;
		int maxSemester = 0;
		
		// Read the input file until the end
		// Create students and houses according to data in input file
		// Determine the max semester 
		while (in.hasNext()) {
			variableType = in.next();
			if (variableType.equals("h")) {
				int id = in.nextInt();
				int duration = in.nextInt();
				double rating = in.nextDouble();
				House house = new House(id, duration, rating);
				houses.add(house);
			}
			else if (variableType.equals("s")) {
				int id = in.nextInt();
				String name = in.next();
				int duration = in.nextInt();
				if (duration > maxSemester)
					maxSemester = duration;
				double rating = in.nextDouble();
				Student student = new Student(id, name, duration, rating);
				students.add(student);
			}
		}
		
		// Array lists in which students and houses are stored temporarily
		ArrayList<Student> studentArr = new ArrayList<Student>();
		ArrayList<House> houseArr = new ArrayList<House>();
		
		// Iterate maxSemester times
		for (int i = 0; i < maxSemester; i++) {
			// Starting from the lowest index iterate for every student
			while (!students.isEmpty()) {
				Student topStudent = students.peek();
				// If a student is graduated then he/she does not need to rent a house
				// Go to the next student
				// We will keep him/her in the priority queue
				// Because at the end of simulation, those who are in the pq will be students who couldn't rent any house at any semester
				if (topStudent.getDuration() <= i) {
					studentArr.add(students.poll());
					continue;
				}
				// Move houses which are rented and have incompatible ratings to the array list
				while (!houses.isEmpty() && (topStudent.getRating() > houses.peek().getRating() || houses.peek().getDuration() != 0))
					houseArr.add(houses.poll());
				// If the priority queue is empty,then there is no suitable house for the student
				// Move them back to priority queue, and move the student to array list temporarily
				if (houses.isEmpty()) {
					while (!houseArr.isEmpty())
						houses.add(houseArr.remove(0));
					studentArr.add(students.poll());
				}
				// If not, the student rents the house and poll the student
				// Update the house duration and move houses in the array back to their priority queue
				else {
					houses.peek().setDuration(topStudent.getDuration()-i);
					students.poll();
					while (!houseArr.isEmpty())
						houses.add(houseArr.remove(0));
				}
			}
			// Move students back to their priority queue to check suitable houses for the next semester
			while (!studentArr.isEmpty())
				students.add(studentArr.remove(0));
			
			// Update the houses' duration
			Iterator<House> it = houses.iterator();
			while (it.hasNext())
				it.next().goNextSemester();
		}
		
		// Write names of students who couldn't rent any house until the end of the simulation
		while (!students.isEmpty())
			out.printf("%s\n", students.poll().getName());
		
		// Close files
		in.close();
		out.close();
	}
}









