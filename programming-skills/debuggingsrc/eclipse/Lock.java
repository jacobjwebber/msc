
/*
 * Debugging exercise for HPC Architectures Course
 * Adrian Jackson, EPCC, 2013
 */

public class Lock{
  
  private boolean isLocked = false;
  
  public synchronized void lock()
  throws InterruptedException{
    while(isLocked){
      wait();
    }
    isLocked = true;
  }
  
  public synchronized void unlock(){
    isLocked = false;
    notify();
  }
}