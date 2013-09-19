package library;

public class Position {

  int i;
  int j;
  int value;

  public Position(int i, int j, int value) {
    this.i = i;
    this.j = j;
    this.value = value;
  }

  public int getValue() {
    return value;
  }

  public void setValue(int value) {
    this.value = value;
  }

  public int getI() {
    return i;
  }

  public void setI(int i) {
    this.i = i;
  }

  public int getJ() {
    return j;
  }

  public void setJ(int j) {
    this.j = j;
  }
}
