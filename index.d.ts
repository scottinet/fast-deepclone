/**
 * @param {T} object - The object to clone.
 * @param {boolean} [copy=false] - Set to "true" to deep copy Buffer, TypedArrays, Map, Set, Date and RegExp objects.
 * @returns {T}
 */
declare function fastDeepClone<T>(object : T, copy? : boolean) : T;

declare module 'fast-deepclone' {
    export = fastDeepClone;
}