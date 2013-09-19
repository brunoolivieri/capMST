package library;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

public class ReadBigFile implements Iterable<Integer> {
  private BufferedReader _reader;

  public ReadBigFile(String fileNameAndPath) throws Exception {
    _reader = new BufferedReader(new FileReader(fileNameAndPath));
  }

  public void Close() {
    try {
      _reader.close();
    }
    catch (Exception ex) {
    }
  }

  @Override
  public Iterator<Integer> iterator() {
    return new ReadBigFileIterator();
  }

  private class ReadBigFileIterator implements Iterator<Integer> {
    private String        currentLine;
    private List<Integer> listValues;

    public ReadBigFileIterator() {
      listValues = new LinkedList<Integer>();
    }

    @Override
    public boolean hasNext() {
      try {
        if (listValues.size() > 0) {
          // Ainda tem algum elemento para ser retornado.
          return true;
        }
        else {
          // Lê uma linha inteira.
          currentLine = _reader.readLine();
          if (currentLine != null) {
            String[] parse = currentLine.trim().split(" ");

            for (int i = 0; i < parse.length; i++) {
              listValues.add(Integer.parseInt(parse[i]));
            }
          }
        }
      }
      catch (Exception ex) {
        currentLine = null;
        ex.printStackTrace();
      }

      // Se a lista tiver tamanho 0 é porque não tem mais nada para ser retornado.
      return (listValues.size() > 0);
    }

    @Override
    public Integer next() {
      if (listValues.size() > 0) {
        return listValues.remove(0);
      }

      return -1;
    }

    @Override
    public void remove() {
    }
  }
}