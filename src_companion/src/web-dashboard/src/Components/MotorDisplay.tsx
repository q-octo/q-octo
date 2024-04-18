import Variable from "./Variable";

/**
 * Each displays Title, Temperature, RPM and Angle of the motor
 */
function MotorDisplay({...props }: MotorDisplayProps) {
    return (
        <div style={{ padding: '10px 32px', width: '200px' }}>
            <h5>{props.title}</h5>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
                <li><Variable title="Temp" value={props.temperature} unit="°C" /></li>
                <li><Variable title="RPM" value={props.rpm} unit="RPM" /></li>
                <li><Variable title="Angle" value={props.angle} unit="°" /></li>
            </ul>
        </div>
    );
}

type MotorDisplayProps = { 
    title: string;
    temperature: number;
    rpm: number;
    angle: number;
}

export default MotorDisplay;