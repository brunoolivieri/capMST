package library;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

public class WriteFile {

  private int    value;
  private String output;

  public WriteFile(String output, int value) {
    this.output = output;
    this.value = value;
  }

  public String getOutput() {
    return output;
  }

  public void setOutput(String output) {
    this.output = output;
  }

  public int getValue() {
    return value;
  }

  public void setValue(int value) {
    this.value = value;
  }

  public void writeFile() throws IOException {

    OutputStreamWriter bufferOut = new OutputStreamWriter(new FileOutputStream(this.output));
    bufferOut.write(String.valueOf(this.value));
    bufferOut.close();
  }
}
