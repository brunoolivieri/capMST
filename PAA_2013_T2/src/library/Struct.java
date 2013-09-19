package library;

public class Struct {

  public enum Situacao {
    NaoAvaliado,
    Menor,
    Rejeitado,
    Excluido
  }

  private int      savingValue;
  private Situacao situacao;

  public Struct(int savingsValue) {
    setSavingValue(savingsValue);
    setSituacao(Situacao.NaoAvaliado);
  }

  public int getSavingValue() {
    return savingValue;
  }

  public void setSavingValue(int savingValue) {
    this.savingValue = savingValue;
  }

  public Situacao getSituacao() {
    return situacao;
  }

  public void setSituacao(Situacao situacao) {
    this.situacao = situacao;
  }

}
