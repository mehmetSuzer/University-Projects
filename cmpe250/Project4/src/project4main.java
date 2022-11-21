

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;


public class project4main {
	public static void main(String[] args) throws FileNotFoundException {
		Scanner in = new Scanner(new File(args[0]));
		PrintStream out = new PrintStream(new File(args[1]));
		
		// Store the capacities of the trains and deers
		int greenTrainNum = in.nextInt();
		int[] greenTrainCaps = new int[greenTrainNum];
		for (int i = 0; i < greenTrainNum; i++)
			greenTrainCaps[i] = in.nextInt();
		
		int redTrainNum = in.nextInt();
		int[] redTrainCaps = new int[redTrainNum];
		for (int i = 0; i < redTrainNum; i++)
			redTrainCaps[i] = in.nextInt();
		
		int greenDeerNum = in.nextInt();
		int[] greenDeerCaps = new int[greenDeerNum];
		for (int i = 0; i < greenDeerNum; i++)
			greenDeerCaps[i] = in.nextInt();
		
		int redDeerNum = in.nextInt();
		int[] redDeerCaps = new int[redDeerNum];
		for (int i = 0; i < redDeerNum; i++)
			redDeerCaps[i] = in.nextInt();
		
		int bagNum = in.nextInt();
		// a: only 1, b: green, c: red, d: train, e: deer
		int bTypeWeight = 0, cTypeWeight = 0, dTypeWeight = 0, eTypeWeight = 0;
		int bdTypeWeight = 0, beTypeWeight = 0, cdTypeWeight = 0, ceTypeWeight = 0;
		HashMap<String, ArrayList<Integer>> aTypes = new HashMap<String, ArrayList<Integer>>();
		int totalGift = 0;
		for (int i = 0; i < bagNum; i++) {
			String type = in.next();
			int giftNum = in.nextInt();
			totalGift += giftNum;
			
			// Combine same types of bags
			if (type.equals("b"))
				bTypeWeight += giftNum;
			else if (type.equals("c"))
				cTypeWeight += giftNum;
			else if (type.equals("d"))
				dTypeWeight += giftNum;
			else if (type.equals("e"))
				eTypeWeight += giftNum;
			else if (type.equals("bd"))
				bdTypeWeight += giftNum;
			else if (type.equals("be"))
				beTypeWeight += giftNum;
			else if (type.equals("cd"))
				cdTypeWeight += giftNum;
			else if (type.equals("ce"))
				ceTypeWeight += giftNum;
			else {
				if (!aTypes.containsKey(type))
					aTypes.put(type, new ArrayList<Integer>());
				aTypes.get(type).add(giftNum);
			}
		}
		int vertexNum = greenTrainNum + redTrainNum + greenDeerNum + redDeerNum + 10;
		int aTypeBagNum = 0;
		for (ArrayList<Integer> weights: aTypes.values()) {
			aTypeBagNum += weights.size();
		}
		vertexNum += aTypeBagNum;
		Graph residualGraph = new Graph(vertexNum);
		
		// Add edges from source to bags
		residualGraph.addEdge(0, 1, bTypeWeight);
		residualGraph.addEdge(0, 2, cTypeWeight);
		residualGraph.addEdge(0, 3, dTypeWeight);
		residualGraph.addEdge(0, 4, eTypeWeight);
		residualGraph.addEdge(0, 5, bdTypeWeight);
		residualGraph.addEdge(0, 6, beTypeWeight);
		residualGraph.addEdge(0, 7, cdTypeWeight);
		residualGraph.addEdge(0, 8, ceTypeWeight);
		
		int startIndex = 9;
		for (String type: aTypes.keySet()) {
			ArrayList<Integer> weights = aTypes.get(type);
			for (int i = 0; i < weights.size(); i++) 
				residualGraph.addEdge(0, startIndex+i, weights.get(i));
			startIndex += weights.size();
		}
		
		// Add edges from bags to trains and deers
		for (int i = 0; i < greenTrainNum; i++) {
			residualGraph.addEdge(1, i+9+aTypeBagNum, bTypeWeight);
			residualGraph.addEdge(3, i+9+aTypeBagNum, dTypeWeight);
			residualGraph.addEdge(5, i+9+aTypeBagNum, bdTypeWeight);
		}
		for (int i = 0; i < greenDeerNum; i++) {
			residualGraph.addEdge(1, i+9+aTypeBagNum+greenTrainNum+redTrainNum, bTypeWeight);
			residualGraph.addEdge(4, i+9+aTypeBagNum+greenTrainNum+redTrainNum, eTypeWeight);
			residualGraph.addEdge(6, i+9+aTypeBagNum+greenTrainNum+redTrainNum, beTypeWeight);
		}
		for (int i = 0; i < redTrainNum; i++) {
			residualGraph.addEdge(2, i+9+aTypeBagNum+greenTrainNum, cTypeWeight);
			residualGraph.addEdge(3, i+9+aTypeBagNum+greenTrainNum, dTypeWeight);
			residualGraph.addEdge(7, i+9+aTypeBagNum+greenTrainNum, cdTypeWeight);
		}
		for (int i = 0; i < redDeerNum; i++) {
			residualGraph.addEdge(2, i+9+aTypeBagNum+greenTrainNum+redTrainNum+greenDeerNum, cTypeWeight);
			residualGraph.addEdge(4, i+9+aTypeBagNum+greenTrainNum+redTrainNum+greenDeerNum, eTypeWeight);
			residualGraph.addEdge(8, i+9+aTypeBagNum+greenTrainNum+redTrainNum+greenDeerNum, ceTypeWeight);
		}
		
		startIndex = 9;
		for (String type: aTypes.keySet()) {
			ArrayList<Integer> weights = aTypes.get(type);
			boolean redRegion = true;
			boolean greenRegion = true;
			boolean carriedByTrain = true;
			boolean carriedByDeer = true;
			for (int j = 1; j < type.length(); j++) {
				if (type.charAt(j) == 'b')
					redRegion = false;
				if (type.charAt(j) == 'c')
					greenRegion = false;
				if (type.charAt(j) == 'd')
					carriedByDeer = false;		
				if (type.charAt(j) == 'e')
					carriedByTrain = false;
			}
			for (int i = 0; i < weights.size(); i++) {
				if (redRegion) {
					if (carriedByTrain) {
						for (int j = 0; j < redTrainNum; j++)
							residualGraph.addEdge(startIndex+i, j+9+aTypeBagNum+greenTrainNum, 1);
					}
					if (carriedByDeer) {
						for (int j = 0; j < redDeerNum; j++)
							residualGraph.addEdge(startIndex+i, j+9+aTypeBagNum+greenTrainNum+redTrainNum+greenDeerNum, 1);
					}
				}
				if (greenRegion) {
					if (carriedByTrain) {
						for (int j = 0; j < greenTrainNum; j++)
							residualGraph.addEdge(startIndex+i, j+9+aTypeBagNum, 1);
					}
					if (carriedByDeer) {
						for (int j = 0; j < greenDeerNum; j++)
							residualGraph.addEdge(startIndex+i, j+9+aTypeBagNum+greenTrainNum+redTrainNum, 1);
					}
				}
			}
			startIndex += weights.size();
		}
		
		// Add edges from train and deers to the sink
		for (int i = 0; i < greenTrainNum; i++)
			residualGraph.addEdge(9+aTypeBagNum+i, vertexNum-1, greenTrainCaps[i]);
		for (int i = 0; i < redTrainNum; i++)
			residualGraph.addEdge(9+aTypeBagNum+greenTrainNum+i, vertexNum-1, redTrainCaps[i]);
		for (int i = 0; i < greenDeerNum; i++)
			residualGraph.addEdge(9+aTypeBagNum+greenTrainNum+redTrainNum+i, vertexNum-1, greenDeerCaps[i]);
		for (int i = 0; i < redDeerNum; i++)
			residualGraph.addEdge(9+aTypeBagNum+greenTrainNum+redTrainNum+greenDeerNum+i, vertexNum-1, redDeerCaps[i]);
		
		// Run Dinic's Algorithm
		int maxFlow = dinicsAlgorithm(residualGraph);
		out.print(totalGift-maxFlow);
		
		in.close();
		out.close();
	}
	
	/**
	 * While there is a path from source to sink, 
	 * it sends flow to sink in the level order.
	 * 
	 * @return maximum flow of the graph
	 */
	public static int dinicsAlgorithm(Graph graph) {
		int totalFlow = 0, flow;
		int[] indices = new int[graph.vertexNum+1];
		while (graph.bfs()) { // while there is a path from source to sink
			for (int i = 0; i < graph.vertexNum+1; i++)
				indices[i] = 0;
			while ((flow = graph.sendFlow(0, Integer.MAX_VALUE, indices)) != 0) // source: 0
				totalFlow += flow;
		}
		return totalFlow;
	}
}



