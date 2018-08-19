using UnityEngine;
using System.Collections;

public class Zombie_cone : MonoBehaviour {

    //Zoombie parent;
	Zoombie parent;
	//bool targetispizza = false;
	// Use this for initialization
	void Start () {
        parent = transform.parent.GetComponent<Zoombie>();
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnTriggerEnter(Collider other)
    {
      //  print(other.name);
        if (other.tag == "Pizza")
        {
            parent.target = other.gameObject;
			collider.enabled = false;
			//targetispizza = true;
        }
        else if (other.tag == "Player")
        {
            parent.target = other.gameObject;
        }
    }
    
    void OnTriggerExit(Collider other)
    {
        if (other.tag == "Pizza")
        {
            parent.target = null;
        }
        else if (other.tag == "Player")
        {
            parent.target = null;
        }
    }
}
