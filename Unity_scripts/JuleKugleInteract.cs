using UnityEngine;
using System.Collections;

public class JuleKugleInteract : MonoBehaviour
{
    private float minForce = 1;
    private float maxForce = 5;

    private float minRotation = 333f;
    private float maxRotation = 400f;

    void Start()
    {

        bool left = (Random.Range(0, 2) == 1);
        float randomForce = Random.Range(minForce, maxForce);
        float randomStartRotation = Random.Range(minRotation, maxRotation);

        transform.rotation = Quaternion.Euler(0, 0, randomStartRotation);

        //print("Rotation: " + randomStartRotation + " Left:" + left + "Force" + randomForce);

        if (left)
            gameObject.rigidbody.AddForce(Vector3.left * randomForce, ForceMode.Impulse);
        else
            gameObject.rigidbody.AddForce(Vector3.right * randomForce, ForceMode.Impulse);
    }
}
