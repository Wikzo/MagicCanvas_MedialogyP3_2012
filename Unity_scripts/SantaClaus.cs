using UnityEngine;
using System.Collections;

public class SantaClaus : MonoBehaviour
{

    public float Speed = 20f;
    private Vector3 StartPosition = new Vector3(-77f, 164f, -5f);
    private Vector3 EndPosition = new Vector3(299f, 164f, -9.5f);
    public GameObject GiftPrefab;
    private bool droppingPackage;
    public GameObject PackageSpawner;
    public AudioClip hoho;

    public static bool SantaMove;

    void Start()
    {
        transform.position = StartPosition;
        SantaMove = false;
        droppingPackage = false;
        GetComponent<AudioSource>().enabled = false;
    }

    IEnumerator SpawnPackage()
    {
        Instantiate(GiftPrefab, PackageSpawner.transform.position, Quaternion.Euler(270, 0, 0));
        float time = Random.Range(0.7f, 1f);
        yield return new WaitForSeconds(time);
        if (droppingPackage == true)
            StartCoroutine(SpawnPackage());
    }

    IEnumerator HoHoSoundEffect()
    {
        float time = Random.Range(7f, 10f);
        yield return new WaitForSeconds(time);
        audio.PlayOneShot(hoho);
    }

    void Update()
    {
        if (SantaMove)
        {
            GetComponent<AudioSource>().enabled = true;
            GetComponentInChildren<ParticleEmitter>().enabled = true;
            Vector3 direction = Vector3.Normalize(EndPosition - transform.position);
            transform.position += direction * Speed * Time.deltaTime;

            if (!droppingPackage)
            {
                droppingPackage = true;
                StartCoroutine(HoHoSoundEffect());
                StartCoroutine(SpawnPackage());
            }

            if (transform.position.x >= EndPosition.x)
            {
                SantaMove = false;
                transform.position = StartPosition;
                TimeManager.GetRandomSantaTime();
            }
        }
        else
        {
            GetComponent<AudioSource>().enabled = false;
            droppingPackage = false;
            GetComponentInChildren<ParticleEmitter>().enabled = false;
        }



        // DEBUG MOVE -------------------------------------
        
        /*
        // Amount to move player LEFT-RIGHT
        float amtToMoveHorizontal = (Speed * Input.GetAxis("Horizontal")) * Time.deltaTime;

        // Move player
        transform.Translate(Vector3.right * amtToMoveHorizontal);

        // Amount to move player UP-DOWN
        float amtToMoveVertical = (Speed * Input.GetAxis("Vertical")) * Time.deltaTime;

        // Move player
        transform.Translate(Vector3.up * amtToMoveVertical);

        */
        // DEBUG MOVE -------------------------------------

    }
}
