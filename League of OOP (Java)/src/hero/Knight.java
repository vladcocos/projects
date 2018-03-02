package hero;

public class Knight extends Hero {
	private float firstAbilityDamage, secondAbilityDamage;
	
	public Knight () {
		super();
	}
	
	public Knight (int maxHp, int count, char heroType, int row, int col) {
		super(maxHp, count, heroType, row, col);
	}
	
	public void GainXp (Hero enemy) {
		super.setXp (super.getXp() + Math.max(0, 200 - (super.getLevel() - enemy.getLevel())*40));
		if (super.getXp() >= 250 + super.getLevel()*50) {
			super.setLevel((int)((super.getXp() - 250)/50) + 1);
			super.setMaxHp(super.getMaxHp() + super.getLevel()*80);
			super.setHp(super.getMaxHp());
		}
	}
	
	public int FirstAbility (Hero enemy, char landType) {
		firstAbilityDamage = 200 + super.getLevel()*30;
		if (enemy.getHp() < Math.min(enemy.getMaxHp()*0.2 + super.getLevel()*0.01, (enemy.getMaxHp()*0.2 + super.getLevel()*0.01)*0.4))
			return enemy.getMaxHp() + 1;
		
		if (landType == 'L')
			firstAbilityDamage += firstAbilityDamage*0.15;
		firstAbilityDamage = Math.round(firstAbilityDamage);

		if (enemy.getHeroType() == 'R')
			firstAbilityDamage += firstAbilityDamage*0.15;
		else if (enemy.getHeroType() == 'P')
			firstAbilityDamage += firstAbilityDamage*0.1;
		else if (enemy.getHeroType() == 'W')
			firstAbilityDamage -= firstAbilityDamage*0.2;
			
		return Math.round(firstAbilityDamage);
	}
	
	public int SecondAbility (Hero enemy, char landType) {	
		secondAbilityDamage = 100 + 40*super.getLevel();
		enemy.setMovementStatus(1);
		
		if (landType == 'L')
			secondAbilityDamage += secondAbilityDamage*0.15;
		secondAbilityDamage = Math.round(secondAbilityDamage);

		if (enemy.getHeroType() == 'R')
			secondAbilityDamage -= secondAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'K')
			secondAbilityDamage += secondAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'P')
			secondAbilityDamage -= secondAbilityDamage*0.1;
		else if (enemy.getHeroType() == 'W')
			secondAbilityDamage += secondAbilityDamage*0.05;

		return Math.round(secondAbilityDamage);	
	}
	
	public int TotalBaseDamage (char landType) {
		if (landType == 'L')
			return (int) Math.round(((300 + super.getLevel())*70*1.15));
		return (300 + super.getLevel()*70);
	}
}
