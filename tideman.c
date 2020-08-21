#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_loop(int top, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    // TODO
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    // TODO
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int temp[2];
    for (int j = 0; j < pair_count - 1; j++)
    {
        for (int i = (j + 1); i < pair_count; i++)
        {
            if ((preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]) > (preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner]))
            {
                temp[0] = pairs[i].winner;
                temp[1] = pairs[i].loser;
                pairs[i] = pairs[j];
                pairs[j].winner = temp[0];
                pairs[j].loser = temp[1];
            }
        }
    }
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!check_loop(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool beaten[candidate_count];
    for (int k = 0; k < candidate_count; k++)
    {
        beaten[k] = false;
    }
    
    for (int j = 0; j < pair_count; j++)
    {
        for (int i = 0; i < pair_count; i++)
        {
            if (locked[j][i] == true)
            {
                beaten[i] = true;
            }
        }
    }
    
    for (int l = 0; l < candidate_count; l++)
    {
        if (beaten[l] == false)
        {
            printf("%s\n", candidates[l]);
        }
    }
    
    // TODO
    return;
}

bool check_loop(int top, int loser)
{

    if (loser == top)
    {
        return true;
    }
    
    for (int i = 0; i < pair_count - 2; i++)
    {
        if (pairs[i].winner == loser)
        {

            if (check_loop(top, pairs[i].loser))
            {
                return true;
            }
        }
    }
    
    return false;
}