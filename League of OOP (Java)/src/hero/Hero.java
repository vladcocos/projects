package hero;

public class Hero {
	private int hp;
	private int maxHp;
	private boolean alive = true;
	private int xp = 0;
	private int heroCount;
	private int level = 0;
	private int movementStatus = 0;
	private int row;
	private int col;
	private int overtimeKill = 0;
	private int overtimeDamage = 0;
	private int overtimeRounds = 0;
	private char heroType;
	
	public Hero () { }
	
	public Hero (int maxHp, int heroCount, char heroType, int row, int col) {
		this.maxHp = maxHp;
		this.hp = maxHp;
		this.heroCount = heroCount;
		this.heroType = heroType;
		this.row = row;
		this.col = col;
	}

	public int getHp() {
		return hp;
	}

	public void setHp(int hp) {
		this.hp = hp;
	}
	
	public int getMaxHp() {
		return maxHp;
	}
	
	public void setMaxHp(int maxHp) {
		this.maxHp = maxHp;
	}
	
	public boolean getAlive() {
		return alive;
	}
	
	public void setAlive(boolean alive) {
		this.alive = alive;
	}

	public int getXp() {
		return xp;
	}

	public void setXp(int xp) {
		this.xp = xp;
	}
	
	public int getHeroCount() {
		return heroCount;
	}
	
	public int getMovementStatus() {
		return movementStatus;
	}

	public void setMovementStatus(int movementStatus) {
		this.movementStatus = movementStatus;
	}

	public int getRow() {
		return row;
	}

	public void setRow(int row) {
		this.row = row;
	}

	public int getCol() {
		return col;
	}

	public void setCol(int col) {
		this.col = col;
	}

	public int getOvertimeKill() {
		return overtimeKill;
	}

	public void setOvertimeKill(int overtimeKill) {
		this.overtimeKill = overtimeKill;
	}

	public int getLevel() {
		return level;
	}
	
	public void setLevel(int level) {
		this.level = level;
	}

	public char getHeroType() {
		return heroType;
	}
	
	public void setHeroType(char heroType) {
		this.heroType = heroType;
	}

	public int getOvertimeDamage() {
		return overtimeDamage;
	}

	public void setOvertimeDamage(int overtimeDamage) {
		this.overtimeDamage = overtimeDamage;
	}

	public int getOvertimeRounds() {
		return overtimeRounds;
	}

	public void setOvertimeRounds(int overtimeRounds) {
		this.overtimeRounds = overtimeRounds;
	}
	
	public int FirstAbility(Hero enemy, char landType) {
		return 0;
	}
	
	public int SecondAbility(Hero enemy, char landType) {
		return 0;
	}
	
	public int TotalBaseDamage(char landType) { 
		return 0;
	}
	
	public void GainXp(Hero enemy) { }
	
}
