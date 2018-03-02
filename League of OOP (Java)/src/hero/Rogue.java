package hero;

public class Rogue extends Hero {
	private float firstAbilityDamage, secondAbilityDamage;
	private int backstabCount = 0;
	
	public Rogue () {
		super();
	}
	
	public Rogue (int maxHp, int count, char heroType, int row, int col) {
		super(maxHp, count, heroType, row, col);
	}
	
	public void GainXp (Hero enemy) {
		super.setXp (super.getXp() + Math.max(0, 200 - (super.getLevel() - enemy.getLevel())*40));
		if (super.getXp() >= 250 + super.getLevel()*50) {
			super.setLevel((int)((super.getXp() - 250)/50) + 1);
			super.setMaxHp(super.getMaxHp() + super.getLevel()*40);
			super.setHp(super.getMaxHp());
		}
	}
	
	public int FirstAbility (Hero enemy, char landType) {
		firstAbilityDamage = 200 + super.getLevel()*20;
		if (backstabCount % 3 == 0 && landType == 'W') {
			firstAbilityDamage = (float) (firstAbilityDamage*1.5);
			backstabCount = 0;
		}
		else
			backstabCount++;
		
		if (landType == 'W')
			firstAbilityDamage += firstAbilityDamage*0.15;
		firstAbilityDamage = Math.round(firstAbilityDamage);
		
		if (enemy.getHeroType() == 'R')
			firstAbilityDamage += firstAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'K')
			firstAbilityDamage -= firstAbilityDamage*0.1;
		else if (enemy.getHeroType() == 'P')
			firstAbilityDamage += firstAbilityDamage*0.25;
		else if (enemy.getHeroType() == 'W')
			firstAbilityDamage += firstAbilityDamage*0.25;
		 
		if (backstabCount == 3)
			backstabCount = 0;
		return Math.round(firstAbilityDamage);
	}
	
	public int SecondAbility (Hero enemy, char landType) {	
		secondAbilityDamage = 40 + super.getLevel()*10;
		
		if (landType == 'W') {
			enemy.setMovementStatus(6);
			enemy.setOvertimeRounds(6);
		}
		else {
			enemy.setMovementStatus(3);
			enemy.setOvertimeRounds(3);
		}
		
		if (landType == 'W')
			secondAbilityDamage += secondAbilityDamage*0.15;
		secondAbilityDamage = Math.round(secondAbilityDamage);
		
		if (enemy.getHeroType() == 'R')
			secondAbilityDamage -= secondAbilityDamage*0.1;
		else if (enemy.getHeroType() == 'K')
			secondAbilityDamage -= secondAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'P')
			secondAbilityDamage += secondAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'W')
			secondAbilityDamage += secondAbilityDamage*0.25;
		
		enemy.setOvertimeDamage(Math.round(secondAbilityDamage));
		return Math.round(secondAbilityDamage);	
	}
	
	public int TotalBaseDamage (char landType) {
		if (landType == 'W')
			return (int) Math.round((240 + super.getLevel()*30)*1.15);
		return (240 + super.getLevel()*30);
	}
}
