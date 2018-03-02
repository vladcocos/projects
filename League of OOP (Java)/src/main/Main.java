package main;

import map.Map;
import java.io.IOException;
import FileIO.implementations.FileReader;
import FileIO.implementations.FileWriter;
import hero.*;
import inputOutput.*;

public class Main {
	public static void main(String[] args) throws IOException {
//		Deschidere fisiere de input si output
		FileReader inputFile = new FileReader(args[0]);
		FileWriter outputFile = new FileWriter(args[1]);
		
//		Citire date de input
		int rowsAux = inputFile.nextInt();
		int colsAux = inputFile.nextInt();

//		Citire harta cu tipuri de land
		char landTypeAux[][] = new char[rowsAux][colsAux];
		Map map = new Map(rowsAux, colsAux, landTypeAux);
		for (int i = 0; i < map.getRows(); i++) {
			String stringAux = inputFile.nextWord();
			for (int j = 0; j < map.getCols(); j++) {
				map.setLandType(i, j, stringAux.charAt(j));
			}
		}
		
//		Citire si initializare eroi
		int numberOfHeroes = inputFile.nextInt();
		Hero[] heroes = new Hero[numberOfHeroes];
		for (int i = 0; i < numberOfHeroes; i++) {
			String stringAux = inputFile.nextWord();
			int row = inputFile.nextInt();
			int col = inputFile.nextInt();
			if (stringAux.charAt(0) == 'P')
				heroes[i] = new Pyromancer (500, i, stringAux.charAt(0), row, col);
			else if (stringAux.charAt(0) == 'K')
				heroes[i] = new Knight (900, i, stringAux.charAt(0), row, col);
			else if (stringAux.charAt(0) == 'W')
				heroes[i] = new Wizard (400, i, stringAux.charAt(0), row, col);
			else if (stringAux.charAt(0) == 'R')
				heroes[i] = new Rogue (600, i, stringAux.charAt(0), row, col);
		}		
				
//		Citire runde si incepere joc
		int rounds = inputFile.nextInt();
		for (int i = 0; i < rounds; i++) {
//			Executa mutari
			map.MoveHeroes(inputFile, heroes, numberOfHeroes);
			
//			Scadere damage overtime, daca exista
			for (int j = 0; j < numberOfHeroes; j++)
				for (int k = 0; k < heroes[j].getOvertimeRounds(); k++) {
					heroes[j].setHp(heroes[j].getHp() - heroes[j].getOvertimeDamage());
					heroes[j].setOvertimeRounds(heroes[j].getOvertimeRounds() - 1);
				}
			
//			Joc propriu-zis
			for (int j = 0; j < numberOfHeroes; j++) {
				for (int k = j + 1; k < numberOfHeroes; k++) {
					char land = map.getLandType(heroes[j].getRow(), heroes[j].getCol());
					
//					Daca exista 2 jucatori pe aceeasi pozitie
					if (heroes[j].getRow() == heroes[k].getRow() && heroes[j].getCol() == heroes[k].getCol() && heroes[j].getAlive() == true && heroes[k].getAlive() == true) {
//						Calculare damage overtime
						if (heroes[k].getOvertimeDamage()*heroes[k].getOvertimeRounds() >= heroes[k].getHp()) {
							heroes[j].setOvertimeKill(1);
							heroes[k].setAlive(false);
						}

						if (heroes[j].getOvertimeDamage()*heroes[j].getOvertimeRounds() >= heroes[j].getHp()) {
							heroes[k].setOvertimeKill(1);
							heroes[j].setAlive(false);
						}

//						Daca unul dintre ei este mort de la overtime, nu se intampla nimic
						if (heroes[j].getOvertimeKill() == 1 || heroes[k].getOvertimeKill() == 1) {
							heroes[j].setOvertimeKill(0);
							heroes[k].setOvertimeKill(0);
							continue;
						}

//						Calculare damage pentru amandoi eroi
						int firstHeroDamage = heroes[j].FirstAbility(heroes[k], land) + heroes[j].SecondAbility(heroes[k], land);
						int secondHeroDamage = heroes[k].FirstAbility(heroes[j], land) + heroes[k].SecondAbility(heroes[j], land);
						
//						Aplicare damage si actualizare hp
						heroes[j].setHp(heroes[j].getHp() - secondHeroDamage);
						heroes[k].setHp(heroes[k].getHp() - firstHeroDamage);
						
//						Lupta s-a terminat
						if (heroes[j].getHp() <= 0)
							heroes[j].setAlive(false);
						
						if (heroes[k].getHp() <= 0)
							heroes[k].setAlive(false);
						
//						Cazul 1: primul este mort
						if (heroes[j].getAlive() == false && heroes[k].getAlive() == true)
							heroes[k].GainXp(heroes[j]);
						
//						Cazul 2: al doilea este mort
						if (heroes[j].getAlive() == true && heroes[k].getAlive() == false)
							heroes[j].GainXp(heroes[k]);
					}
				}
			}
		}
			
//		Scriere output
		Output writeOutput = new Output ();
		writeOutput.WriteOutput(outputFile, heroes, numberOfHeroes);
		
		inputFile.close();
		outputFile.close();
	}
}