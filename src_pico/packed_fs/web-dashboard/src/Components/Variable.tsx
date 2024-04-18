/**
 * Use .variable styles for this component
 */
function Variable<T>({ ...props }: VariableProps<T>) {
    return (
        <div className="variable" style={{ display: 'flex', justifyContent:'space-around', flexDirection: 'row', gap: '8px' }}>
            {props.title && <div><strong>{props.title}: </strong></div>}
            <div><em>{props.value} {props.unit}</em></div>
        </div>
    );
}

type VariableProps<T> = {
    title?: string;
    value: T;
    unit?: string;
}

export default Variable;