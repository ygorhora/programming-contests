package com.urionlinejudge.problem1464;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.StringTokenizer;

/**
 * https://www.urionlinejudge.com.br/judge/pt/problems/view/1464
 * @title Camada de Cebolas
 * @themes Convex Hull, Geometry 
 * @author Ygor Hora
 */
public class Main {
	
	public static void main(String[] args) throws IOException {
		
		Reader sc = new Reader();
		PrintWriter out = new PrintWriter(new BufferedOutputStream(System.out));
		
		int nPoints;
		LinkedList<Point2D> pointSet;
		
		while((nPoints = sc.nextInt()) != 0){
			
			pointSet = new LinkedList<>();
			
			for(int i = 0; i < nPoints; ++i){
				Double x = sc.nextDouble();
				Double y = sc.nextDouble();
				pointSet.add(new Point2D(x,y));
			}
			
			ConvexHull cvh = new ConvexHull();
			
			int nLayers = 0;
			
			do{
				cvh.runMonotoneChain(pointSet, true);
				pointSet = cvh.getRemainder();
				nLayers++;
			} while(pointSet.size() > 0);
			
			if(nLayers % 2 == 1){
				out.println("Take this onion to the lab!");
			} else {
				out.println("Do not take this onion to the lab!");
			}
		}
		
		out.flush();
		out.close();
	}
}

class ConvexHull {
	private static final Double EPS = 1.0e-7;
	LinkedList<Point2D> remainder;
	
	public LinkedList<Point2D> getRemainder() {
		return this.remainder;
	}

	/**
	 * @param pointSet a list of points
	 * @param addCongruentPoints true if to consider the congruents points
	 * @return the set of points in convex hull
	 */
	public LinkedList<Point2D> runMonotoneChain(LinkedList<Point2D> pointSet, Boolean addCongruentPoints){
		Collections.sort(pointSet);
		LinkedList<Point2D> ll = findLowerLayer(pointSet, addCongruentPoints);
		
		this.remainder = getDifference(pointSet.iterator(),ll.iterator());
		this.remainder.addFirst(pointSet.getFirst());
		this.remainder.addLast(pointSet.getLast());
		
		LinkedList<Point2D> up = findUpperLayer(remainder, addCongruentPoints);
		
		this.remainder = getDifference(remainder.iterator(), up.descendingIterator());
		
		return mergeLayers(ll,up);
	}
	
	public LinkedList<Point2D> getDifference(Iterator<Point2D> universalSet, Iterator<Point2D> withdrawSet) {
		LinkedList<Point2D> ans = new LinkedList<>(); 
		
		Point2D i = withdrawSet.next();
		
		while(universalSet.hasNext()){
			Point2D p = universalSet.next();
			if(!i.equals(p)){
				ans.add(p);
			} else {
				i = withdrawSet.hasNext()? withdrawSet.next(): null;
			}
		}
		
		return ans;
	}

	public LinkedList<Point2D> mergeLayers(LinkedList<Point2D> ll, LinkedList<Point2D> up) {
		LinkedList<Point2D> ans = new LinkedList<>();
		for(int i = 0; i < ll.size() - 1; ++i){
			ans.add(ll.get(i));
		}
		
		for(int j = 0; j < up.size() - 1; j++){
			ans.add(up.get(j));
		}
		
		return ans;
	}
	
	/**
	 * @param pointSet an ordered list of points
	 * @param addCongruentPoints true if to consider the congruents points
	 * @return the points of lower layer that are in convex hull
	 */
	public LinkedList<Point2D> findLowerLayer(LinkedList<Point2D> pointSet, Boolean addCongruentPoints){
		return findLayer(pointSet, true, addCongruentPoints);
	}
	
	/**
	 * @param pointSet an ordered list of points
	 * @param addCongruentPoints true if to consider the congruents points
	 * @return the points of lower layer that are in convex hull
	 */
	public LinkedList<Point2D> findUpperLayer(LinkedList<Point2D> pointSet, Boolean addCongruentPoints){
		return findLayer(pointSet, false, addCongruentPoints); 
	}
	
	/**
	 * @param pointSet an ordered list of points
	 * @param isLowerLayer calculate lower layer if true or upper layer if false
	 * @param addCongruentPoints true if to consider the congruents points
	 * @return the points of indicated layer that are in convex hull
	 */
	private LinkedList<Point2D> findLayer(LinkedList<Point2D> pointSet, Boolean isLowerLayer, Boolean addCongruentPoints){
		Point2D[] stackAns = new Point2D[pointSet.size()]; // the stack answer will be at most all the points
		int pntPostEndStack = 0;
		int pntStartStack = 0;
		
		Iterator<Point2D> itr = isLowerLayer? pointSet.iterator() : pointSet.descendingIterator();

		while(itr.hasNext()){

			Point2D p2 = itr.next();

			while(pntPostEndStack - pntStartStack >= 2){
				
				Point2D p1 = stackAns[pntPostEndStack-1];
				Point2D p0 = stackAns[pntPostEndStack-2];

				Point2D v1 = Point2D.getVector(p0, p1);
				Point2D v2 = Point2D.getVector(p0, p2);

				int signal = v1.crossSignal(v2, EPS);

				if((signal < 0) || (signal == 0 && !addCongruentPoints)){
					pntPostEndStack--; // implicitly removing a point from stack					
				} else {
					
					break; // the point will be included in convex hull					
				}
			}

			stackAns[pntPostEndStack++] = p2;
			
		}
		
		return new LinkedList<Point2D>(Arrays.asList(Arrays.copyOfRange(stackAns, 0, pntPostEndStack)));
	}
}

class Point2D implements Comparable<Point2D>{
	private Double x;
	private Double y;
	
	public Point2D(Double x, Double y){
		this.x = x;
		this.y = y;
	}

	@Override
	public int compareTo(Point2D p){
		int cmpx = this.getX().compareTo(p.getX());
		if(cmpx == 0){
			return this.getY().compareTo(p.getY());
		}
		
		return cmpx;
	}
	
	public int crossSignal(Point2D p, Double absError){
		Double crossProd = cross(p);
		
		if(crossProd > -1 * Math.abs(absError) && crossProd < Math.abs(absError))
			return 0;
				
		return (int) Math.signum(crossProd);
	}
	
	public Double cross(Point2D p){
		return this.getX() * p.getY() - this.getY() * p.getX();
	}
	
	public static Point2D getVector(Point2D src, Point2D dest){
		return new Point2D(dest.getX() - src.getX(), dest.getY() - src.getY());
	}
	
	public boolean equals(Point2D p){
		return this.getX() == p.getX() && this.getY() == p.getY(); 
	}
	
	public Double getX(){
		return this.x;
	}
	
	public Double getY(){
		return this.y;
	}
	
	public String toString(){
		return "x: " + getX() + " y: " + getY();
	}
}

class Reader {
	private BufferedReader br;
	private StringTokenizer st;
	
	public Reader(){
		br = new BufferedReader(new InputStreamReader(System.in));
	}
	
	private String next(){
		while (st == null || !st.hasMoreElements()){
			try {
				String line = br.readLine();
				st = new StringTokenizer(line);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return st.nextToken();
	}
	
	public int nextInt(){
		return Integer.parseInt(next());
	}
	
	public long nextLong(){
		return Long.parseLong(next());
	}
	
	public double nextDouble(){
		return Double.parseDouble(next());
	}
	
	public String nextLine(){
		String str = "";
		try {
			str = br.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return str;
	}
}