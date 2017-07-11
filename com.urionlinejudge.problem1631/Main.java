package com.urionlinejudge.problem1631;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.StringTokenizer;

/**
 * https://www.urionlinejudge.com.br/judge/pt/problems/view/1631
 * @title O Fantástico Bolo de Bobby
 * @theme Geometry
 * @author Ygor Hora
 */
public class Main {
	
	public static void main(String[] args) throws IOException {
		Reader sc = new Reader();
		PrintWriter out = new PrintWriter(new BufferedOutputStream(System.out));
		
		int nPoints;
		
		while((nPoints = sc.nextInt()) != 0){
			ArrayList<Point2D> points = new ArrayList<>();
			
			for(int i = 0; i < nPoints; i++){
				Double x = sc.nextDouble();
				Double y = sc.nextDouble();
				points.add(new Point2D(x, y));
			}
			
			Double menorDist = 200.0;
			
			for(int i = 1; i < nPoints; i++){
				Point2D fixedPoint = points.get(i);
				Point2D vector1;
				
				for(int j = 0; j < i; j++){
					vector1 = Point2D.getVector(fixedPoint, points.get(j));
					
					int lado;
					double vectorProd, dist1 = 0.0, dist2 = 0.0;
					Point2D vector2;
					
					// process distance not divide by norm.
					for(int k = 0; k < nPoints; k++){
						vector2 = Point2D.getVector(fixedPoint, points.get(k));
						lado = vector1.crossSignal(vector2, 1.0e-7);
						vectorProd = vector1.cross(vector2);
						switch(lado){
							case -1:
								dist1 += Math.abs(vectorProd);
								break;
							case 1:
								dist2 += Math.abs(vectorProd);
								break;
						}
					}
					menorDist = Math.min(Math.abs(dist1-dist2)/vector1.getNorm(), menorDist);
				}
			}
			out.println(String.format("%.3f", menorDist));
		}
		
		out.flush();
		out.close();
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
	
	public Double getNorm(){
		return Math.sqrt(this.x*this.x+this.y*this.y);
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
