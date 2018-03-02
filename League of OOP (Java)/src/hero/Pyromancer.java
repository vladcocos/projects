package hero;

public class Pyromancer extends Hero {
	private float firstAbilityDamage, secondAbilityDamage, overtimeDamage;
	
	public Pyromancer () {
		super();
	}
	
	public Pyromancer (int maxHp, int count, char heroType, int row, int col) {
		super(maxHp, count, heroType, row, col);
	}
	
	public void GainXp (Hero enemy) {
		super.setXp (super.getXp() + Math.max(0, 200 - (super.getLevel() - enemy.getLevel())*40));
		if (super.getXp() >= 250 + super.getLevel()*50) {
			super.setLevel((int)((super.getXp() - 250)/50) + 1);
			super.setMaxHp(super.getMaxHp() + super.getLevel()*50);
			super.setHp(super.getMaxHp());
		}
	}
	
	public int FirstAbility (Hero enemy, char landType) {
		firstAbilityDamage = 350 + 50*super.getLevel();
		if (landType == 'V')
			firstAbilityDamage += firstAbilityDamage*0.25;
		firstAbilityDamage = Math.round(firstAbilityDamage);
		
		if (enemy.getHeroType() == 'R')
			firstAbilityDamage -= firstAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'K')
			firstAbilityDamage += firstAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'P')
			firstAbilityDamage -= firstAbilityDamage*0.1;
		else if (enemy.getHeroType() == 'W')
			firstAbilityDamage += firstAbilityDamage*0.05;
		 
		return Math.round(firstAbilityDamage);
	}
	
	public int SecondAbility (Hero enemy, char landType) {	
		secondAbilityDamage = 150 + super.getLevel()*20;
		overtimeDamage = 50 + super.getLevel()*30;
		if (landType == 'V') {
			secondAbilityDamage += secondAbilityDamage*0.25;
			overtimeDamage += overtimeDamage*0.25;
		}
		secondAbilityDamage = Math.round(secondAbilityDamage);
		overtimeDamage = Math.round(overtimeDamage);
		
		if (enemy.getHeroType() == 'R') {
			secondAbilityDamage -= secondAbilityDamage*0.2;
			overtimeDamage -= overtimeDamage*0.2;
		}
		else if (enemy.getHeroType() == 'K') {
			secondAbilityDamage += secondAbilityDamage*0.2;
			overtimeDamage += overtimeDamage*0.2;
		}
		else if (enemy.getHeroType() == 'P') {
			secondAbilityDamage -= secondAbilityDamage*0.1;
			overtimeDamage -= overtimeDamage*0.1;
		}
		else if (enemy.getHeroType() == 'W') {
			secondAbilityDamage += secondAbilityDamage*0.05;
			overtimeDamage += overtimeDamage*0.05;
		}
	
		enemy.setOvertimeDamage(Math.round(overtimeDamage));
		enemy.setOvertimeRounds(2);
		return Math.round(secondAbilityDamage);	
	}
	
	public int TotalBaseDamage (char landType) {
		if (landType == 'V')
			return (int) Math.round(((500 + super.getLevel())*70*1.25));
		return (500 + super.getLevel()*70);
	}
}
