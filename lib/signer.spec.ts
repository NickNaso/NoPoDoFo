import * as test from 'tape'
import { writeFileSync } from 'fs'
import { Document } from './document'
import { SignatureField } from './field'
import { NPDFAnnotation, NPDFAnnotationFlag } from './annotation'
import { Rect } from './rect'
import { join } from 'path'
import { Form } from "./form";
import { Signer, signature } from "./signer";
import { Data } from './data';


const doc = new Document(join(__dirname, '../test-documents/test.pdf'), true)
doc.on('ready', async e => {
    if (e instanceof Error) throw e
    try {
        // check for form
        let form = new Form(doc),
            formDict = form.getObject().asDictionary()
        if (!formDict.hasKey('SigFlags') ||
            formDict.getKey('SigFlags').type !== 'Number' ||
            formDict.getKey('SigFlags').asNumber() !== 3) {
            if (formDict.hasKey('SigFlags'))
                formDict.removeKey('SigFlags')
            formDict.addKey('SigFlags', 3)
        }
        if (form.needAppearances)
            form.needAppearances = false

        const rect = new Rect([0, 0, 10, 10]),
            page = doc.getPage(1),
            annot = page.createAnnotation(NPDFAnnotation.Widget, rect)
        annot.flag = NPDFAnnotationFlag.Hidden | NPDFAnnotationFlag.Invisible
        const field = new SignatureField(annot, form, doc),
            signatureData = signature(join(__dirname, '../test-documents/certificate.pem'), join(__dirname, '../test-documents/key.pem'))
        field.setReason('test')
        field.setLocation('here')
        field.setCreator('me')
        field.setFieldName('signer.sign')
        field.setDate()

        let signer = new Signer(doc)
        signer.setField(field)
        let signedPath = "/tmp/signed.pdf"
        signer.signSync(signatureData, signedPath)
        test('happy path digital signature', t => {
            let signed = new Document(signedPath)
            signed.on('ready', e => {
                if (e instanceof Error) t.fail(e.message)
                const signedPage = doc.getPage(1),
                    fields = signedPage.getFieldsInfo()
                let signatureFieldCandidates = fields.filter(i => i.name === 'signer.sign')
                if(!signatureFieldCandidates || signatureFieldCandidates.length === 0) t.fail("signature field not found")
                else if(signatureFieldCandidates.length === 1) {
                    t.pass("signature found")
                    t.end()
                }
                else t.fail("something went wrong")
            })
        })
    }
    catch
        (e) {
        console.error(e)
    }
})

