using UnityEngine;
using System.Collections;

public class Ninja_attack : MonoBehaviour {
    Ninja_throw parent;
    // Use this for initialization
    void Start () {
        parent = transform.parent.GetComponent<Ninja_throw>();
    }
    
    // Update is called once per frame
    void Update () 
    {
    }
    
    void OnTriggerEnter(Collider other)
    {
 
         if (other.tag == "Player")
        {
            parent.target = other.gameObject;
        }
    }
    
    void OnTriggerExit(Collider other)
    {

         if (other.tag == "Player")
        {
            parent.target = null;
        }
    }
}
