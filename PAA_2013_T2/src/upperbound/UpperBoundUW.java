package upperbound;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import library.Position;
import library.ReadBigFile;
import library.Struct;
import library.Struct.Situacao;
import library.WriteFile;

public class UpperBoundUW {
  private final static boolean DEBUG        = false;
  private static String        input        = "data/TE16.txt";  //TE16.txt; TE_rafael.TXT; TE_Teste.txt
  private static String        output       = "data/result.TXT";
  private static int           maxCapacity  = 3;
  private static int           mergeNumbers = 0;
  private static int           cmstCost     = 0;
  private static int           root         = 0;
  private int[]                capacity;                        //     = new int[] { 1, 1, 1, 2, 1, 1, 1 };
  private List<Integer>        groups;
  private List<Integer>        ranks;
  private Set<Integer>         mergedNodes;

  /**
   * @param args
   * @throws Exception
   */
  public static void main(String[] args) throws Exception {
    if (!DEBUG) {
      if (args.length != 3)
        throw new Exception("Parâmetros inválidos!");

      maxCapacity = Integer.parseInt(args[0]);
      input = args[1];
      output = args[2];
    }
    new UpperBoundUW().run();
  }

  public void run() throws IOException {

    if (DEBUG) {
      maxCapacity = 3;
      System.out.println("Capacidade:" + maxCapacity);
      execute();

      maxCapacity = 5;
      System.out.println("Capacidade:" + maxCapacity);
      execute();

      maxCapacity = 10;
      System.out.println("Capacidade:" + maxCapacity);
      execute();
    }
    else {
      execute();
    }
  }

  private void execute() throws IOException {
    this.groups = new ArrayList<Integer>();
    this.ranks = new ArrayList<Integer>();
    this.cmstCost = 0;
    this.mergeNumbers = 0;

    // obtendo o número de verticies
    int numVertices = getNumeroVertices();
    root = numVertices;
    numVertices++;
    this.mergedNodes = new HashSet<Integer>(numVertices);

    montarCapacidade(numVertices);
    // inicializando a matriz de distancias
    Integer[][] matriz = new Integer[numVertices][numVertices];
    montarMatrizDistancia(numVertices, matriz);
    //imprimirMatriz(matriz);
    // inicializando a savings matriz
    Struct[][] savingsMatriz = new Struct[numVertices][numVertices];
    montarSavingsMatriz(savingsMatriz, matriz, numVertices);
    //imprimirMatriz(savingsMatriz);
    // montando os grupos
    makeSet(numVertices);
    while (mergeNumbers < numVertices - 1) {
      Position min = findMinimum(savingsMatriz);
      if (min.getI() == -1) {
        break;
      }
      // Se for válido e não fecha ciclo
      if (isValid(min, matriz)) {
        //System.out.println("Merge de " + min.getI() + " com " + min.getJ());
        merge(min, matriz, savingsMatriz);
        savingsMatriz[min.getI()][min.getJ()].setSituacao(Situacao.Excluido);
        //imprimirMatriz(savingsMatriz);
        mergeNumbers++;
        cmstCost += matriz[min.getI()][min.getJ()];
      }
      else {
        savingsMatriz[min.getI()][min.getJ()].setSituacao(Situacao.Rejeitado);
      }
      //System.out.println(cont);
    }
    // Adicionando as arestas ao root.
    calcularcustoArestasRoot(matriz, numVertices);
    if (DEBUG)
      System.out.println("CMST upperBound: " + cmstCost);

    WriteFile writeFile = new WriteFile(this.output, this.cmstCost);
    writeFile.writeFile();
  }

  private void calcularcustoArestasRoot(Integer[][] matriz, int numVertices) {
    //int min = Integer.MAX_VALUE - 1;
    //int pos = 0;
    //int ini = 1;
    //ArrayList<Integer> array = new ArrayList<Integer>();
    //if (maxCapacity == 1) {
    //  for (int i = 1; i < groups.size(); i++) {
    //if (i != root) {
    //   cmstCost += matriz[0][i];
    // }
    // }
    // }
    //else {
    ArrayList<Integer> marked = new ArrayList<Integer>();
    for (int i = 1; i < groups.size(); i++) {
      if (groups.get(i) != -1 && i != root && !marked.contains(groups.get(i))) {
        marked.add(groups.get(i));

        int min = matriz[0][i];
        for (int j = i; j < groups.size(); j++) {
          if (j != root && i != j && groups.get(i) == groups.get(j) || j == groups.get(i)) {
            if (matriz[0][j] < min) {
              min = matriz[0][j];
            }
          }
        }
        cmstCost += min;
      }
    }
    for (int i = 1; i < groups.size(); i++) {
      if (i != root && !marked.contains(groups.get(i)) && groups.get(i).equals(-1)) {
        boolean finded = false;
        for (int j = i + 1; j < groups.size(); j++) {
          if (groups.get(j).equals(i)) {
            finded = true;
          }
        }
        if (!finded && !marked.contains(i))
          cmstCost += matriz[0][i];
      }
      // }
    }
    //for (int i = ini; i < matriz.length; i++) {
    //  if (i != root /* && matriz[0][i] < min */) {
    //    array.add(matriz[0][i]);
    //  }
    //if (!mergedNodes.contains(i))
    //  cmstCost += matriz[0][i];
    //}
    //Collections.sort(array);
    //int delta = (int) Math.ceil((float) numVertices / maxCapacity);
    //delta += 2;
    //if (delta > array.size()) {
    //  delta = array.size() - 1;
    // System.out.println("mior");
    // }
    //if (mergedNodes.size() > 0)
    //  delta = 16 - mergedNodes.size() + 1;
    //else
    //  delta = array.size();

    //for (int i = 0; i < delta; i++) {
    //System.out.print(array.get(i) + " ");
    //  cmstCost += array.get(i);
    //}
  }

  private void montarCapacidade(int numVertices) {
    capacity = new int[numVertices];
    for (int i = 0; i < capacity.length; i++) {
      capacity[i] = 1;
    }
  }

  private boolean isValid(Position min, Integer[][] matriz) {
    int sum = 0;
    int groupI = -2;
    int groupJ = -2;

    if (min.getI() == root || min.getJ() == root)
      return false;

    // Desconsiderando caso o root esteja em questão
    //if (min.getI() != root)
    groupI = find(min.getI());

    //if (min.getJ() != root)
    groupJ = find(min.getJ());

    // Se não fecha ciclo
    if (groupI == groupJ)
      return false;

    for (int i = 0; i < matriz.length; i++) {
      if (i == root)
        continue;

      int aux = find(i);
      if (groupI == aux || groupJ == aux) {
        sum += capacity[i];
      }
    }

    return sum <= maxCapacity;
  }

  private Position findMinimum(Struct[][] savingsMatriz) {
    int min = Integer.MAX_VALUE;
    int x = -1;
    int y = -1;

    for (int i = 0; i < savingsMatriz.length; i++) {
      for (int j = 0; j < savingsMatriz.length; j++) {
        if (i != j && savingsMatriz[i][j].getSavingValue() < min) {
          if (!savingsMatriz[i][j].getSituacao().equals(Situacao.Excluido) && !savingsMatriz[i][j].getSituacao().equals(Situacao.Rejeitado)) {
            min = savingsMatriz[i][j].getSavingValue();
            x = i;
            y = j;
          }
        }
      }
    }
    //savingsMatriz[x][y].setSituacao(Situacao.Excluido);
    return new Position(x, y, min);
  }

  private void merge(Position min, Integer[][] matriz, Struct[][] savingsMatriz) {
    if (savingsMatriz[0][min.getJ()].getSavingValue() < savingsMatriz[0][min.getI()].getSavingValue()) {
      for (int j = 1; j < savingsMatriz.length; j++) {
        savingsMatriz[min.getI()][j].setSavingValue(matriz[j][min.getI()] - matriz[0][min.getJ()]);
      }
    }
    union(min.getI(), min.getJ());
    this.mergedNodes.add(min.getI());
    this.mergedNodes.add(min.getJ());
  }

  private void montarSavingsMatriz(Struct[][] savingsMatriz, Integer[][] matriz, int numVertices) {
    for (int i = 0; i < matriz.length; i++) {
      for (int j = 0; j < matriz.length; j++) {
        savingsMatriz[i][j] = new Struct(matriz[i][j] - matriz[i][root]);
      }
    }
  }

  private void imprimirMatriz(Integer[][] matriz) {
    System.out.println("------------------------");
    for (int i = 0; i < matriz.length; i++) {
      for (int j = 0; j < matriz.length; j++) {
        System.out.print(matriz[i][j] + "       ");
      }
      System.out.println();
    }
    System.out.println("------------------------");
  }

  private void imprimirMatriz(Struct[][] matriz) {
    System.out.println("------------------------");
    for (int i = 0; i < matriz.length; i++) {
      for (int j = 0; j < matriz.length; j++) {
        System.out.print(matriz[i][j].getSavingValue() + "       ");
      }
      System.out.println();
    }
    System.out.println("------------------------");
  }

  private void montarMatrizDistancia(int numVertices, Integer[][] matriz) {
    ReadBigFile file = null;
    try {
      file = new ReadBigFile(input);
      Iterator<Integer> iterator = file.iterator();
      iterator.hasNext();
      iterator.next();
      int i = 0, j = 0;
      while (iterator.hasNext()) {
        matriz[i][j] = iterator.next();
        j++;
        if (j == numVertices) {
          i++;
          j = 0;
        }
      }
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    finally {
      file.Close();
    }
  }

  public int getNumeroVertices() {
    ReadBigFile file = null;
    int result = -1;
    try {
      file = new ReadBigFile(input);
      Iterator<Integer> iterator = file.iterator();
      iterator.hasNext();
      result = iterator.next();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    finally {
      file.Close();
    }
    return result;
  }

  private void makeSet(int numVertices) {
    for (int i = 0; i < numVertices; i++) {
      groups.add(-1);
      ranks.add(0);
    }
  }

  private int union(Integer one, Integer two) {
    int u = find(one);
    int v = find(two);

    if (ranks.get(one).compareTo(ranks.get(two)) > 0) {
      groups.set(v, u);
      return u;
    }
    else if (ranks.get(two).compareTo(ranks.get(one)) > 0) {
      groups.set(u, v);
      return v;
    }
    else {
      groups.set(v, u);
      ranks.set(one, ranks.get(one) + 1);
    }
    return u;
  }

  private Integer find(Integer node) {
    return groups.get(node).equals(-1) ? node : find(groups.get(node));
  }
}
