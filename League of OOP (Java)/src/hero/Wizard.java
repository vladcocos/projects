package hero;

public class Wizard extends Hero {
	private float firstAbilityDamage, secondAbilityDamage;
	
	public Wizard () {
		super();
	}
	
	public Wizard (int maxHp, int count, char heroType, int row, int col) {
		super(maxHp, count, heroType, row, col);
	}
	
	public void GainXp (Hero enemy) {
		super.setXp (super.getXp() + Math.max(0, 200 - (super.getLevel() - enemy.getLevel())*40));
		if (super.getXp() >= 250 + super.getLevel()*50) {
			super.setLevel((int)((super.getXp() - 250)/50) + 1);
			super.setMaxHp(super.getMaxHp() + super.getLevel()*30);
			super.setHp(super.getMaxHp());
		}
	}

	public int FirstAbility (Hero enemy, char landType) {
		firstAbilityDamage = (float) (0.2 + super.getLevel()*0.05);
		
		if (landType == 'D')
			firstAbilityDamage += firstAbilityDamage*0.1;
		
		if (enemy.getHeroType() == 'R')
			firstAbilityDamage -= firstAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'K')
			firstAbilityDamage += firstAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'P')
			firstAbilityDamage -= firstAbilityDamage*0.1;
		else if (enemy.getHeroType() == 'W')
			firstAbilityDamage += firstAbilityDamage*0.05;
		 
		return (int) Math.round(firstAbilityDamage*Math.min(enemy.getMaxHp()*0.3, enemy.getHp()));
	}
	
	public int SecondAbility (Hero enemy, char landType) {
		if (enemy.getHeroType() != 'W')
			secondAbilityDamage = (float) Math.min(enemy.TotalBaseDamage(landType)*0.35 + super.getLevel()*0.02, 
					(enemy.TotalBaseDamage(landType)*0.35 + super.getLevel()*0.02)*0.7);
		else
			return 0;
		
		if (landType == 'D')
			secondAbilityDamage += secondAbilityDamage*0.1;
		secondAbilityDamage = Math.round(secondAbilityDamage);
		
		if (enemy.getHeroType() == 'R')
			secondAbilityDamage += secondAbilityDamage*0.2;
		else if (enemy.getHeroType() == 'K')
			secondAbilityDamage += secondAbilityDamage*0.4;
		else if (enemy.getHeroType() == 'P')
			secondAbilityDamage += secondAbilityDamage*0.3;
		 
		return Math.round(secondAbilityDamage);	
	}
	
	public int TotalBaseDamage (Hero enemy, char landType) {
		if (landType == 'D')
			return (int) Math.round(((0.2 + super.getLevel()*0.05)*1.1)*Math.min(enemy.getMaxHp()*0.3, enemy.getHp()));
		return (int) Math.round((0.2 + super.getLevel()*0.05)*Math.min(enemy.getMaxHp()*0.3, enemy.getHp()));
	}
}
