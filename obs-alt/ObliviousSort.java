import java.io.*;
import java.lang.*;
import java.util.*;

/** A Very Basic Sequential Version of Bucket Oblivious Sort (Java Edition) **/
public class ObliviousSort {
    /* --- Bucket Size --- */
    private static int Z = 128;

    /* 
     * --- Bucket Element ---
     * data - type defined as integer
     * key - randomly assigned through butterfly network
     * type - NORMAL | FILLER 
     */
    private static class Element {
        int data;
        String key;
        String type;
    }

    /*
     * --- Get Number of Arrays ---
     * n2 - twice the number of input elements
     * Z - bucket size
     * Output - log B
     */
    private static int findArrays(int n2, int Z) {
        int minSize = n2 / Z + (n2 % Z == 0 ? 0 : 1);

        if (minSize == 0) return 0;

        return (int) Math.ceil(Math.log(minSize) / Math.log(2));
    }

    /*
     * --- Check for Bucket Overflows ---
     */ 
    private static void checkOverflow(int index, int max) {
        if (index >= max) {
            throw new java.lang.Error("Bucket Overflow");
        }
    }

    /*
     * --- MergeSplit ---
     * Ai, Aj - jth pair of buckets with distance 2^i
     * i - row of butterfly network
     */
    private static Element[][] mergeSplit(Element[] Ai, Element[] Aj, int i) {
        Element[] A0 = new Element[Z];
        Element[] A1 = new Element[Z];

        // Pad A′0 and A′1 to size Z with dummy elements and return (A′0,A′1)
        Element filler = new Element();
        filler.data = -1;
        filler.key = "\0";
        filler.type = "FILLER";
        Arrays.fill(A0, filler);
        Arrays.fill(A1, filler);

        int m = 0;
        int n = 0;

        /* A0 receives all real elements in Ai U Aj where the (i + 1)-st MSB of the key is 0
         * A1 receives all real elements in Ai U Aj where the (i + 1)-st MSB of the key is 1
         * If either A0 or A1 receives more than Z real elements, the procedure aborts with overflow */

        // sort elements from A0
        for (int k = 0; k < Ai.length; k++) {
            if ((Ai[k].type).equals("FILLER")) {
                break;
            }

            char bucket = (Ai[k].key).charAt(i);
            if (bucket == '0') {
                A0[m] = Ai[k];
                m++;
            } else {
                A1[n] = Ai[k];
                n++;
            }

            checkOverflow(m, Z);
            checkOverflow(n, Z);
        }

        // sort elements from A1
        for (int k = 0; k < Aj.length; k++) {
            if ((Aj[k].type).equals("FILLER")) {
                break;
            }

            char bucket = (Aj[k].key).charAt(i);
            if (bucket == '0') {
                A0[m] = Aj[k];
                m++;
            } else {
                A1[n] = Aj[k];
                n++;
            }

            checkOverflow(m, Z);
            checkOverflow(n, Z);
        }

        Element[][] results = new Element[2][Ai.length];
        results[0] = A0;
        results[1] = A1;

        return results;
    }

    /*
     * --- Algorithm 3.1: Oblivious Random Bin Assignment ---
     * Input:
     *   X - integer array; n - size of X
     * Output:
     *   A - array of buckets containing obliviously distributed elements
     */ 
    public static Element[][] orba(int[] X, int n) {
        // Random Number Generator
        Random rand = new Random();

        int numArrays = findArrays(2 * n, Z);
        int B = (int) Math.pow(2, numArrays);

        // define butterfly network
        Element[][][] A = new Element[numArrays + 1][B][Z];

        // initialize filler value
        Element filler = new Element();
        filler.data = -1;
        filler.key = "\0";
        filler.type = "FILLER";

        // fill buckets with real elements
        int e_per_b = n / B + (n % B == 0 ? 0 : 1);
        for (int i = 0; i < n; i++) {
            // determine bucket and placement
            int bucket = i / e_per_b;
            int currElements = i % e_per_b;

            // assign random key
            int randKey = rand.nextInt(B);

            // assign data element
            Element new_element = new Element();
            new_element.data = X[i];
            new_element.key = String.format("%32s", Integer.toBinaryString(randKey)).replace(' ', '0');
            new_element.key = new_element.key.substring(new_element.key.length() - numArrays);
            new_element.type = "NORMAL";

            A[0][bucket][currElements] = new_element;
        }

        // pad buckets with filler elements to size Z
        for (int i = 0; i < B; i++) {
            for (int j = 0; j < Z; j++) {
                if (A[0][i][j] == null) {
                    A[0][i][j] = filler;
                }
            }
        }

        // route elements through butterfly network
        for (int i = 0; i < numArrays; i++) { // level of network
            int powTwoI = (int) Math.pow(2, i);
            for (int j = 0; j < B / 2; j++) { // pair of inputs routed
                // calculate j'
                int j_prime = (int) (Math.floor(j / powTwoI) * powTwoI);

                // determine indices of buckets
                int first_bucket = j_prime + j;
                int second_bucket = j_prime + powTwoI + j;

                // perform sort by indices
                Element[][] mergedA = mergeSplit(A[i][first_bucket], A[i][second_bucket], i);

                // assign buckets to next layer of network
                A[i + 1][2 * j] = mergedA[0];
                A[i + 1][2 * j + 1] = mergedA[1];
            }
        }

        // output: A(log B) = A_0(log B) || ... A_{B_1}(log B)
        return A[numArrays];
    }


    /* 
     * --- 3.2 Bucket Oblivious Random Permutation ---
     * Input:
     *   X - integer array; n - size of X
     * Output:
     *   concatenated array of obliviously sorted elements
     */
    public static Element[] orp(Element[][] X, int n) {
        Element[] permuted = new Element[n];
        int curr_index = 0;

        for (int i = 0; i < X.length; i++) {
            int curr_n = 0;
            List<Element> soln = new ArrayList<Element>();

            // scan the array and delete the dummy elements from each bin
            // (real elements appear before dummy elements)
            for (int k = 0; k < X[i].length; k++) {
                if (Objects.equals(X[i][k].type, ("FILLER"))) {
                    break;
                }
                soln.add(X[i][k]);
                curr_n++;
            }

            // randomly permute bin
            Collections.shuffle(soln);

            // concatenate all bins
            for (int j = curr_index; j < Math.min(n, curr_index + curr_n); j++) {
                permuted[j] = soln.get(j - curr_index);
            }

            curr_index += curr_n;
        }

        return permuted;
    }

    /*
     * --- Quick Sort ---
     */
    private static int partition(int[] arr, int begin, int end) {
        int pivot = arr[end];
        int i = (begin-1);
    
        for (int j = begin; j < end; j++) {
            if (arr[j] <= pivot) {
                i++;
    
                int swapTemp = arr[i];
                arr[i] = arr[j];
                arr[j] = swapTemp;
            }
        }
    
        int swapTemp = arr[i+1];
        arr[i+1] = arr[end];
        arr[end] = swapTemp;
    
        return i+1;
    }

    public static void quickSort(int[] arr, int begin, int end) {
        if (begin < end) {
            int partitionIndex = partition(arr, begin, end);
    
            quickSort(arr, begin, partitionIndex-1);
            quickSort(arr, partitionIndex+1, end);
        }
    }

    /**
     * --- Bucket Oblivious Sort ---
     * calls the three parts of Bucket Oblivious Sort on an array of integers
     **/
    public static int[] obs(int[] arr, int n) {
        Element[][] result = orba(arr, n);
        Element[] permutedResult = orp(result, arr.length);

        // get data from element struct
        int[] sorted = new int[n];
        for (int i = 0; i < n; i++) {
            sorted[i] = permutedResult[i].data;
        }

        quickSort(sorted, 0, n - 1);
        return sorted;
    }

    private static void checkElements(int[] sorted) {
        int prev = 0;
        for (int i: sorted) {
            if (i != prev + 1) {
                throw new java.lang.Error("Sorting Error");
            } else {
                prev += 1;
            }
        }
    }

    /**
     * Tests for Bucket Oblivious Sort
     * -- all permutations of numbers below 16
     * -- randomly generates and tests for higher input arrays
     */
    public static void test() {
        Random rand = new Random();

        File file = new File("permutations.txt");
        List<String> tests = new ArrayList<String>();

        try {
            Scanner input = new Scanner(file);
            while (input.hasNextLine()) {
                tests.add(input.nextLine());
            }
            input.close();
        } catch (FileNotFoundException ex) {
            tests.add("01234567");
        }
        List<int[]> perms = new ArrayList<int[]>();
        for (int i = 0; i < tests.size(); i++) {
            String curr_e = tests.get(i);

            int[] curr_as_int = new int[curr_e.length()];
            for (int k = 0; k < curr_e.length(); k++) {
                curr_as_int[k] = Integer.parseInt(String.valueOf(curr_e.charAt(k)));
            }

            perms.add(curr_as_int);
        }

        /** Permutations for 1 through 16 */
        for (int[] test_in : perms) {
            int[] sorted = obs(test_in, test_in.length);
            checkElements(sorted);
        }

        /** Random Selections for 17 and above */
        int[] lst = new int[1];
        for (int epoch = 0; epoch < 100; epoch++) {
            int n = rand.nextInt(500000);
            lst = new int[n];
            for (int i = 0; i < n; i++) 
                lst[i] = i + 1;
            Collections.shuffle(Arrays.asList(lst));

            int[] sorted = obs(lst, lst.length);
            checkElements(sorted);
            // System.out.println(epoch);
        }

        return;
    }

    public static void main(String args[]) {
        test();
        return;
    }
}