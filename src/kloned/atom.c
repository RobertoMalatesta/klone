#include <klone/utils.h>
#include <klone/queue.h>
#include <klone/atom.h>

int atom_create(const char *id, const char *data, size_t size, void* arg, 
    atom_t **patom)
{
    atom_t *atom = NULL;

    atom = u_calloc(sizeof(atom_t));
    dbg_err_if(atom == NULL);

    atom->id = u_strdup(id);
    dbg_err_if(atom->id == NULL);

    atom->data = u_memdup(data, size);
    dbg_err_if(atom->data == NULL);

    atom->size = size;
    atom->arg = arg;

    *patom = atom;

    return 0;
err:
    if(atom)
        atom_free(atom);
    return ~0;
}

int atom_free(atom_t* atom)
{
    dbg_err_if(atom == NULL);

    if(atom->id)
        u_free(atom->id);

    if(atom->data)
        u_free(atom->data);

    u_free(atom);

    return 0;
err:
    return ~0;
}

/* sum of atoms size field */
size_t atoms_size(atoms_t *as)
{
    dbg_err_if(as == NULL);

    return as->size;
err:
    return -1;
}

/* # of atoms */
size_t atoms_count(atoms_t *as)
{
    dbg_err_if(as == NULL);

    return as->count;
err:
    return -1;
}

/* return the n-th atom */
int atoms_getn(atoms_t *as, int n, atom_t **patom)
{
    atom_t *atom;
    int i = 0;

    dbg_err_if(as == NULL || n >= as->count || patom == NULL);

    LIST_FOREACH(atom, &as->list, np)
    {
        if(i++ == n)
        {
            *patom = atom;
            return 0;
        }
    }

err:
    return ~0; /* out of bounds */
}

/* return the atom whose ID is id */
int atoms_get(atoms_t *as, const char *id, atom_t **patom)
{
    atom_t *atom;

    dbg_err_if(as == NULL || id == NULL || patom == NULL);

    LIST_FOREACH(atom, &as->list, np)
    {
        if(strcmp(id, atom->id) == 0)
        {
            *patom = atom;
            return 0;
        }
    }

err:
    return ~0; /* not found */
}

/* add an atom to the list */
int atoms_add(atoms_t *as, atom_t *atom)
{
    dbg_err_if(as == NULL || atom == NULL);

    LIST_INSERT_HEAD(&as->list, atom, np);

    as->count++;
    as->size += atom->size;

    return 0;
err:
    return ~0;
}

/* add an atom to the list */
int atoms_remove(atoms_t *as, atom_t *atom)
{
    dbg_err_if(as == NULL || atom == NULL);

    LIST_REMOVE(atom, np);

    as->count--;
    as->size -= atom->size;

    return 0;
err:
    return ~0;

}

/* create an atom list */
int atoms_create(atoms_t **pas)
{
    atoms_t *as = NULL;

    dbg_err_if(pas == NULL);

    as = u_calloc(sizeof(atoms_t));
    dbg_err_if(as == NULL);

    LIST_INIT(&as->list);

    *pas = as;

    return 0;
err:
    if(as)
        u_free(as);
    return ~0;
}


/* free an atom list */
int atoms_free(atoms_t *as)
{
    atom_t *atom;

    dbg_err_if(as == NULL);

    while(atoms_count(as))
    {
        dbg_err_if(atoms_getn(as, 0, &atom));
        dbg_err_if(atoms_remove(as, atom));
    }

    u_free(as);

    return 0;
err:
    return ~0;
}
