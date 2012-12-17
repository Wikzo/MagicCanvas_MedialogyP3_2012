using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using UnityEngine;
using System.Collections;
using Random = UnityEngine.Random;

public class CharacterManager : MonoBehaviour
{
    private bool _fileExists;
    public float Character_Z_Default = -5f;
    public float Character_Y_Default = 100f;

    private float Snowman_Y_Min = 58.5f;
    private float Snowman_Y_Max = 75f;

    private float NisseGirl_Y_Min = 47f;
    private float NisseGirl_Y_Max = 60f;

    private float NisseBoy_Y_Min = 52.7f;
    private float NisseBoy_Y_Max = 81.3f;

    private float Angel_Y_Max = 155f;
    private float Angel_Y_Min = 123f;

    private float Squirrel_Y_Min = 50f;
    private float Squirrel_Y_Max = 71f;

    public GameObject[] CharacterPrefab;
    private int _characterNumber;

    public List<GameObject> Characters;
    private const float BACKGROUND_WIDTH = 220;
    private string _text;
    Vector3 _pixieDefaultPosition;
    private Vector3 outOfPicture_left = new Vector3(-22f, 90f, -5f);
    private Vector3 outOfPicture_right = new Vector3(240f, 90f, -5f);
    private const float REFRESH_TIME = 0.1f;
    //  private float[] _xPositions;

    private string[] ID;
    private float[] Position;
    private Vector3[] target;
    private bool[] wasInvalidLastFrame;

    private bool readyForNewTarget;

    //public int _numberOfPeopleRightNow;
    private float[] pointX;

    private static PropertyInfo _mSystemCopyBufferProperty = null;
    public float MoveSpeed = 80f;

    private float MovingThreshold = 2;

    private float timer;
    private const float WaitTimeToGetNewTarget = 0.1f;

    private bool inputError;
    private bool[] characterHasBeenSwitched;

    private char[] c;

    private int numberToSpawn = 10;

    private List<int> listOfThreeLastCharacters;

    //public AudioClip[] magic;

    // Use this for initialization
    void Start()
    {
        _mSystemCopyBufferProperty = null;
        listOfThreeLastCharacters = new List<int>();
        _pixieDefaultPosition = new Vector3(0, Character_Y_Default, Character_Z_Default);
        _characterNumber = 0;

        c = new char[1];
        c[0] = 'i'; // used to split and not get empty string back

        inputError = false;
        characterHasBeenSwitched = new bool[numberToSpawn];
        ID = new string[numberToSpawn];
        Position = new float[numberToSpawn];
        wasInvalidLastFrame = new bool[numberToSpawn];

        SpawnCharacters(numberToSpawn);
        GetNewTarget();
        timer = WaitTimeToGetNewTarget;

        for (int i = 0; i < 10; i++)
            characterHasBeenSwitched[i] = false;


    }

    // Update is called once per frame
    void Update()
    {
        GetReadyToMove();
        SmoothMove();

        // Update target timer
        if (timer < WaitTimeToGetNewTarget)
        {
            timer += Time.deltaTime;
        }
        else if (timer >= WaitTimeToGetNewTarget)
        {
            timer = 0;
            GetNewTarget();
        }

    }

    void SpawnCharacters(int count)
    {
        GetDataFromClipBoard();
        Characters = new List<GameObject>();
        for (int i = 0; i < count; i++)
        {
            GetRandomCharacter();
            GameObject temp = (GameObject)Instantiate(CharacterPrefab[_characterNumber], _pixieDefaultPosition, Quaternion.Euler(270f, 0, 0));

            if (i >= ID.Length)
                temp.name = "ID_waitingForInput_" + i;
            else
                temp.name = ID[i];

            Characters.Add(temp);
        }
    }

    void GetRandomCharacter()
    {
        int randomCharacter = Random.Range(0, CharacterPrefab.Length);
        bool isSame = true;
        int tries = 0;

        while (isSame == true && tries < 10)
        {
            for (int i = 0; i < listOfThreeLastCharacters.Count; i++)
            {
                if (listOfThreeLastCharacters[i] != randomCharacter)
                    isSame = false;
            }

            randomCharacter = Random.Range(0, CharacterPrefab.Length);
            tries++;
        }

        if (listOfThreeLastCharacters.Count < 3)
            listOfThreeLastCharacters.Add(randomCharacter);
        else
        {
            listOfThreeLastCharacters.RemoveAt(0);
            listOfThreeLastCharacters.Add(randomCharacter);
        }

        _characterNumber = randomCharacter;
    }

    void OnGUI()
    {
        if (inputError)
            GUI.Label(new Rect(50, 50, 500, 200), "FEJL! - Der er noget galt med ClipBoard manageren.\n(Tryk Escape for at afslutte programmet)");
    }

    void GetDataFromClipBoard()
    {
        _text = ClipBoard; // raw data from clipboard


        // new

        // Check to see if OpenCV program is running and feeding us with the correct data
        if (!_text.StartsWith("q") && !_text.EndsWith("q"))
        {
            inputError = true;
            //throw new InvalidOperationException("ERROR - There is something wrong with the ClipBoard function data! Are you sure the OpenCV program is running?");
        }
        else
            inputError = false;

        // WE START WITH THIS DATA:
        // q i5p0.5 i42p0.5 i6p0.6 i7p0.341 i123p-1 q
        // [start character: q] [id tag: i] [person coordinate: p] [end character: q]
        string noWhiteSpace = _text.Replace(" ", string.Empty);
        string[] removeStartAndEnd = noWhiteSpace.Split('q'); // to get away with blank space

        // now we have: i5p0.5i6p0.6i7p0.7
        string[] tempIDs = removeStartAndEnd[1].Split(c, StringSplitOptions.RemoveEmptyEntries);

        // now we have: string array ID with names + float array Position with position values
        for (int i = 0; i < ID.Length; i++)
        {
            string[] temp = tempIDs[i].Split('p'); // split into IDs AND Positions
            ID[i] = "ID_" + temp[0]; // Save ID name
            float.TryParse(temp[1], out Position[i]); // Get position in float format

            /*if (Position[i] > 0f)
                Position[i] = 1f - Position[i]; // flip so it is not mirrored*/
        }

    }

    void GetNewTarget()
    {
        target = new Vector3[Position.Length];
        
        for (int i = 0; i < Characters.Count; i++)
        {
            if (Position[i] == -1) // don't give a target to an invalid character
            {
                //Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.NoAnimation;

                if (Characters[i].transform.position.x >= 110)
                {
                    target[i] = outOfPicture_right;
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                }
                else
                {
                    target[i] = outOfPicture_left;
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                }

                continue;
            }

            //print(target[0]);

            if (Characters[i].tag == "Angel")
            {
                target[i] = new Vector3(Position[i] * BACKGROUND_WIDTH, Random.Range(Angel_Y_Min, Angel_Y_Max),
                                        Character_Z_Default);
                Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
            }
            else if (Characters[i].tag == "Snowman")
            {
                target[i] = new Vector3(Position[i] * BACKGROUND_WIDTH, Random.Range(Snowman_Y_Min, Snowman_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;
            }
            else if (Characters[i].tag == "Nisseboy")
            {
                target[i] = new Vector3(Position[i] * BACKGROUND_WIDTH, Random.Range(NisseBoy_Y_Min, NisseBoy_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;
            }
            else if (Characters[i].tag == "Nissegirl")
            {
                target[i] = new Vector3(Position[i] * BACKGROUND_WIDTH, Random.Range(NisseGirl_Y_Min, NisseGirl_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;
            }
            else if (Characters[i].tag == "Squirrel")
            {
                target[i] = new Vector3(Position[i] * BACKGROUND_WIDTH, Random.Range(Squirrel_Y_Min, Squirrel_Y_Max),
                                        Character_Z_Default);

                // Change moving sprite
                if (target[i].x > (Characters[i].transform.position.x + MovingThreshold)) // right
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingRight;
                else if (target[i].x < (Characters[i].transform.position.x - MovingThreshold)) // left
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.MovingLeft;
                else // static
                    Characters[i].GetComponent<AwesomeSpriteSheetAnimatorCS>().State = SpriteState.Static;

            }

            if (wasInvalidLastFrame[i] == true)
            {
                /*if (i == 0)
                    audio.PlayOneShot(magic[Random.Range(0, magic.Length)]);*/
                Characters[i].transform.position = target[i];
                wasInvalidLastFrame[i] = false;
            }
        }
    }

    void SmoothMove()
    {
        for (int i = 0; i < Characters.Count; i++)
        {
            if (target[i].x > Characters[i].transform.position.x + MovingThreshold
                    || target[i].x < Characters[i].transform.position.x - MovingThreshold)
            {
                // due to time.DeltaTime in a for loop, more characters will faster than few characters
                Vector3 direction = Vector3.Normalize(target[i] - Characters[i].transform.position);
                Characters[i].transform.position += direction * MoveSpeed * Time.deltaTime;
            }
        }
    }

    void GetReadyToMove()
    {
        GetDataFromClipBoard();

        /*if (Position.Length < Characters.Count)
        {
            inputError = true;
            //throw new InvalidOperationException("ERROR - input data is smaller than amount of characters in the list");
        }
        else if (Position.Length > Characters.Count)
        {
            inputError = true;
            //throw new InvalidOperationException("ERROR - input data is larger than amount of characters in the list");
        }
        else
            inputError = false;*/

        // Update the pixie's positions
        for (int i = 0; i < Characters.Count; i++)
        {
            //SmoothMove();
            if (Position[i] == -1f)
            {
                
                Characters[i].name = ID[i] + "_invalid";

                // if outside picture
                if (Characters[i].transform.position.x >= outOfPicture_right.x - 2 ||
                    Characters[i].transform.position.x <= outOfPicture_left.x + 2)
                {
                    if (characterHasBeenSwitched[i] == false)
                    {
                        wasInvalidLastFrame[i] = true;
                        GameObject oldCharacter = Characters[i];
                        GetRandomCharacter();
                        GameObject newCharacter = (GameObject)Instantiate(CharacterPrefab[_characterNumber], oldCharacter.transform.position,
                                        Quaternion.Euler(270f, 0, 0));

                        Characters[i] = newCharacter;

                        characterHasBeenSwitched[i] = true;
                        Destroy(oldCharacter.renderer.material);
                        Destroy(oldCharacter);
                    }
                }
            }
            else
            {
                characterHasBeenSwitched[i] = false;
                Characters[i].name = ID[i];
            }
        }
    }

    private static PropertyInfo GetSystemCopyBufferProperty()
    {
        if (_mSystemCopyBufferProperty == null)
        {
            Type T = typeof(GUIUtility);
            _mSystemCopyBufferProperty = T.GetProperty("systemCopyBuffer", BindingFlags.Static | BindingFlags.NonPublic);
            if (_mSystemCopyBufferProperty == null)
                throw new Exception("Can't access internal member 'GUIUtility.systemCopyBuffer' it may have been removed / renamed");
        }
        return _mSystemCopyBufferProperty;
    }

    public static string ClipBoard
    {
        get
        {
            PropertyInfo P = GetSystemCopyBufferProperty();
            return (string)P.GetValue(null, null);
        }
        set
        {
            PropertyInfo P = GetSystemCopyBufferProperty();
            P.SetValue(null, value, null);
        }
    }
}
